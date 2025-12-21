#include "LocalFileReader.h"
#include <stdexcept>

namespace furry_toy {

void LocalFileReader::open(const std::string& filename, std::ios_base::openmode mode) {
    m_fileStream.open(filename, mode);
    if (!m_fileStream) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}

void LocalFileReader::close() {
    m_fileStream.close();
}

bool LocalFileReader::is_open() const {
    return m_fileStream.is_open();
}

std::streamsize LocalFileReader::read(char* s, std::streamsize n) {
    m_fileStream.read(s, n);
    m_lastGcount = m_fileStream.gcount();
    return m_lastGcount;
}

std::optional<std::string> LocalFileReader::getLine() {
    std::string line;
    if (std::getline(m_fileStream, line)) {
        return line;
    }
    return std::nullopt;
}

FileReader& LocalFileReader::getline(std::string& str, char delim) {
    std::getline(m_fileStream, str, delim);
    return *this;
}

FileReader& LocalFileReader::seekg(std::streampos pos, std::ios_base::seekdir dir) {
    m_fileStream.seekg(pos, dir);
    return *this;
}

std::streampos LocalFileReader::tellg() {
    return m_fileStream.tellg();
}

std::streamsize LocalFileReader::gcount() const {
    return m_lastGcount;
}

int LocalFileReader::peek() {
    return m_fileStream.peek();
}

int LocalFileReader::get() {
    return m_fileStream.get();
}

FileReader& LocalFileReader::ignore(std::streamsize n, int delim) {
    m_fileStream.ignore(n, delim);
    return *this;
}

std::streamsize LocalFileReader::readsome(char* s, std::streamsize n) {
    return m_fileStream.readsome(s, n);
}

FileReader& LocalFileReader::putback(char ch) {
    m_fileStream.putback(ch);
    return *this;
}

FileReader& LocalFileReader::unget() {
    m_fileStream.unget();
    return *this;
}

LocalFileReader::operator bool() const {
    return is_open();
}

} // namespace furry_toy