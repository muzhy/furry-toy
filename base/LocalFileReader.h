#ifndef __FURRY_TOY_LOCAL_FILE_READER_H__
#define __FURRY_TOY_LOCAL_FILE_READER_H__

#include "FileReader.h"
#include <fstream>

namespace furry_toy {

/**
 * @brief LocalFileReader 类，实现本地文件的读取操作。
 * 
 * 该类继承自 FileReader，使用 std::ifstream 进行本地文件读取。
 */
class LocalFileReader : public FileReader {
public:
    LocalFileReader() = default;
    virtual ~LocalFileReader() override = default;

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
    std::ifstream m_fileStream;  ///< 本地文件流
    std::streamsize m_lastGcount = 0;  ///< 上次读取的字符数
};

} // namespace furry_toy

#endif // __FURRY_TOY_LOCAL_FILE_READER_H__