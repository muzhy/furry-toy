#include "MongoDBFileReader.h"
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <stdexcept>
#include <algorithm>

namespace furry_toy {

MongoDBFileReader::MongoDBFileReader(std::string_view dbName, std::string_view bucketName, std::string_view fileId)
    : m_dbName(dbName), m_bucketName(bucketName), m_fileId(fileId) {
    // 初始化MongoDB实例（假设全局）
    static mongocxx::instance instance{};
}

void MongoDBFileReader::open(const std::string& filename, std::ios_base::openmode mode) {
    try {
        // 连接到MongoDB（假设默认URI）
        m_client = mongocxx::client{mongocxx::uri{}};
        m_database = m_client[m_dbName];
        m_bucket = m_database.gridfs_bucket({.bucket_name = m_bucketName});

        // 打开下载流
        auto downloadStream = m_bucket.open_download_stream(m_fileId);
        m_downloader = std::make_unique<mongocxx::gridfs::downloader>(std::move(downloadStream));

        // 获取文件信息
        auto fileDoc = m_bucket.find_one(m_fileId);
        if (!fileDoc) {
            throw std::runtime_error("File not found in GridFS: " + m_fileId);
        }
        auto length = fileDoc->view()["length"].get_int64();
        const std::size_t chunkSize = 256 * 1024; // 默认chunk大小
        m_totalChunks = (length + chunkSize - 1) / chunkSize;

        // 加载第一个chunk
        loadChunk(0);
        m_chunkPos = 0;
        m_isOpen = true;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to open MongoDB file: " + std::string(e.what()));
    }
}

void MongoDBFileReader::close() {
    m_downloader.reset();
    m_currentChunk.clear();
    m_isOpen = false;
}

bool MongoDBFileReader::is_open() const {
    return m_isOpen;
}

std::streamsize MongoDBFileReader::read(char* s, std::streamsize n) {
    std::streamsize totalRead = 0;
    while (n > 0 && m_chunkIndex < m_totalChunks) {
        std::streamsize available = m_currentChunk.size() - m_chunkPos;
        std::streamsize toRead = std::min(n, available);
        if (toRead > 0) {
            std::copy(m_currentChunk.begin() + m_chunkPos, m_currentChunk.begin() + m_chunkPos + toRead, s);
            s += toRead;
            m_chunkPos += toRead;
            n -= toRead;
            totalRead += toRead;
        }
        if (n > 0 && m_chunkIndex + 1 < m_totalChunks) {
            // 保存当前状态以便异常恢复
            auto savedChunkIndex = m_chunkIndex;
            auto savedChunkPos = m_chunkPos;
            try {
                loadChunk(++m_chunkIndex);
                m_chunkPos = 0;
            } catch (...) {
                // 恢复状态
                m_chunkIndex = savedChunkIndex;
                m_chunkPos = savedChunkPos;
                throw;
            }
        } else if (n > 0) {
            break; // 文件结束
        }
    }
    m_lastGcount = totalRead;
    return totalRead;
}

std::optional<std::string> MongoDBFileReader::getLine() {
    std::string line;
    char ch;
    while (read(&ch, 1) > 0) {
        if (ch == '\n') {
            return line;
        }
        line += ch;
    }
    if (!line.empty()) {
        return line;
    }
    return std::nullopt;
}

FileReader& MongoDBFileReader::getline(std::string& str, char delim) {
    str.clear();
    char ch;
    while (read(&ch, 1) > 0) {
        if (ch == delim) {
            break;
        }
        str += ch;
    }
    return *this;
}

FileReader& MongoDBFileReader::seekg(std::streampos pos, std::ios_base::seekdir dir) {
    std::streamoff offset = pos;
    if (dir == std::ios_base::cur) {
        offset += tellg();
    } else if (dir == std::ios_base::end) {
        // 需要文件长度，但GridFS文件长度未知，简化处理
        throw std::runtime_error("seekg from end not supported");
    }
    if (offset < 0) {
        throw std::runtime_error("Invalid seek position");
    }
    std::size_t targetPos = static_cast<std::size_t>(offset);
    const std::size_t chunkSize = 256 * 1024;
    std::size_t targetChunk = targetPos / chunkSize;
    std::size_t targetChunkPos = targetPos % chunkSize;

    if (targetChunk != m_chunkIndex) {
        loadChunk(targetChunk);
        m_chunkIndex = targetChunk;
    }
    m_chunkPos = targetChunkPos;
    return *this;
}

std::streampos MongoDBFileReader::tellg() {
    const std::size_t chunkSize = 256 * 1024;
    return static_cast<std::streampos>(m_chunkIndex * chunkSize + m_chunkPos);
}

std::streamsize MongoDBFileReader::gcount() const {
    return m_lastGcount;
}

int MongoDBFileReader::peek() {
    if (m_chunkPos >= m_currentChunk.size()) {
        if (m_chunkIndex + 1 >= m_totalChunks) {
            return EOF;
        }
        // 临时加载下一个chunk
        auto savedIndex = m_chunkIndex;
        auto savedPos = m_chunkPos;
        try {
            loadChunk(m_chunkIndex + 1);
            int ch = m_currentChunk[0];
            loadChunk(savedIndex); // 恢复
            m_chunkPos = savedPos;
            return ch;
        } catch (...) {
            return EOF;
        }
    }
    return m_currentChunk[m_chunkPos];
}

int MongoDBFileReader::get() {
    char ch;
    if (read(&ch, 1) > 0) {
        return ch;
    }
    return EOF;
}

FileReader& MongoDBFileReader::ignore(std::streamsize n, int delim) {
    for (std::streamsize i = 0; i < n; ++i) {
        int ch = get();
        if (ch == EOF || ch == delim) {
            break;
        }
    }
    return *this;
}

std::streamsize MongoDBFileReader::readsome(char* s, std::streamsize n) {
    return read(s, n);
}

FileReader& MongoDBFileReader::putback(char ch) {
    if (m_chunkPos > 0) {
        --m_chunkPos;
    }
    return *this;
}

FileReader& MongoDBFileReader::unget() {
    return putback(0); // 简化
}

MongoDBFileReader::operator bool() const {
    return m_isOpen;
}

void MongoDBFileReader::loadChunk(std::size_t index) {
    if (index >= m_totalChunks) {
        throw std::runtime_error("Chunk index out of range");
    }
    m_currentChunk.resize(256 * 1024); // 假设chunk大小
    std::streamsize bytesRead = m_downloader->read(m_currentChunk.data(), m_currentChunk.size());
    m_currentChunk.resize(bytesRead);
}

} // namespace furry_toy