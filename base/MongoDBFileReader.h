#ifndef __FURRY_TOY_MONGODB_FILE_READER_H__
#define __FURRY_TOY_MONGODB_FILE_READER_H__

#include "FileReader.h"
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/gridfs/bucket.hpp>
#include <mongocxx/gridfs/downloader.hpp>
#include <string>
#include <vector>
#include <memory>

namespace furry_toy {

/**
 * @brief MongoDBFileReader 类，实现从MongoDB数据库中读取文件的操作，使用GridFS。
 * 
 * 该类继承自 FileReader，从指定的GridFS bucket中读取文件内容。
 */
class MongoDBFileReader : public FileReader {
public:
    /**
     * @brief 构造函数。
     * @param dbName 数据库名称。
     * @param bucketName bucket名称。
     * @param fileId 文件ID。
     */
    explicit MongoDBFileReader(std::string_view dbName, std::string_view bucketName, std::string_view fileId);

    virtual ~MongoDBFileReader() override = default;

    void open(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in) override;
    void close() override;
    bool is_open() const override;
    std::streamsize read(char* s, std::streamsize n) override;
    std::optional<std::string> getLine() override;
    FileReader& getline(std::string& str, char delim = '\n') override;
    FileReader& seekg(std::streampos pos, std::ios_base::seekdir dir = std::ios_base::beg) override;
    std::streampos tellg() override;
    std::streamsize gcount() const override;
    int peek() override;
    int get() override;
    FileReader& ignore(std::streamsize n = 1, int delim = EOF) override;
    std::streamsize readsome(char* s, std::streamsize n) override;
    FileReader& putback(char ch) override;
    FileReader& unget() override;
    operator bool() const override;

private:
    std::string m_dbName;          ///< 数据库名称
    std::string m_bucketName;      ///< bucket名称
    std::string m_fileId;          ///< 文件ID

    mongocxx::client m_client;     ///< MongoDB客户端
    mongocxx::database m_database; ///< 数据库
    mongocxx::gridfs::bucket m_bucket; ///< GridFS bucket
    std::unique_ptr<mongocxx::gridfs::downloader> m_downloader; ///< 下载器

    std::vector<std::uint8_t> m_currentChunk; ///< 当前加载的chunk数据
    std::size_t m_chunkIndex = 0;   ///< 当前chunk索引
    std::size_t m_totalChunks = 0;  ///< 总chunk数量
    std::size_t m_chunkPos = 0;     ///< 当前chunk内的位置
    std::streamsize m_lastGcount = 0; ///< 上次读取的字符数
    bool m_isOpen = false;          ///< 是否已打开

    /**
     * @brief 加载指定索引的chunk。
     * @param index chunk索引。
     */
    void loadChunk(std::size_t index);

    /**
     * @brief 检查是否需要加载下一个chunk。
     * @param neededBytes 需要读取的字节数。
     * @return true 如果需要加载，否则 false。
     */
    bool needNextChunk(std::streamsize neededBytes) const;
};

} // namespace furry_toy

#endif // __FURRY_TOY_MONGODB_FILE_READER_H__