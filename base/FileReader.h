#ifndef __FURRY_TOY_FILE_READER_H__
#define __FURRY_TOY_FILE_READER_H__

#include <string>
#include <optional>
#include <ios>
#include <string_view>

namespace furry_toy {

/**
 * @brief 抽象基类 FileReader，提供统一的读取文件接口，参考 std::fstream 但只支持读取操作。
 * 
 * 该类定义了文件读取的基本操作，包括打开、关闭、读取、定位等。
 * 派生类需要实现所有纯虚函数。
 */
class FileReader {
public:
    FileReader() = default;
    virtual ~FileReader() = default;

    // 禁止拷贝和赋值
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    /**
     * @brief 打开文件。
     * @param filename 文件名或标识符。
     * @param mode 打开模式（参考 std::ios_base::openmode）。
     */
    virtual void open(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in) = 0;

    /**
     * @brief 关闭文件。
     */
    virtual void close() = 0;

    /**
     * @brief 检查文件是否打开。
     * @return true 如果文件已打开，否则 false。
     */
    virtual bool is_open() const = 0;

    /**
     * @brief 读取数据到缓冲区。
     * @param s 缓冲区。
     * @param n 要读取的字节数。
     * @return 读取的字节数。
     */
    virtual std::streamsize read(char* s, std::streamsize n) = 0;

    /**
     * @brief 按行读取文件内容。
     * @return 读取的行内容，如果到达文件末尾则返回 std::nullopt。
     */
    virtual std::optional<std::string> getLine() = 0;

    /**
     * @brief 读取一行到字符串（类似 std::getline）。
     * @param str 存储读取的字符串。
     * @param delim 分隔符，默认为 '\n'。
     * @return 引用自身。
     */
    virtual FileReader& getline(std::string& str, char delim = '\n') = 0;

    /**
     * @brief 设置读取位置。
     * @param pos 位置。
     * @param dir 方向（参考 std::ios_base::seekdir）。
     * @return 引用自身。
     */
    virtual FileReader& seekg(std::streampos pos, std::ios_base::seekdir dir = std::ios_base::beg) = 0;

    /**
     * @brief 获取当前读取位置。
     * @return 当前位置。
     */
    virtual std::streampos tellg() = 0;

    /**
     * @brief 获取上次读取操作读取的字符数。
     * @return 字符数。
     */
    virtual std::streamsize gcount() const = 0;

    /**
     * @brief 查看下一个字符而不移动位置。
     * @return 下一个字符，如果失败则返回 EOF。
     */
    virtual int peek() = 0;

    /**
     * @brief 读取一个字符。
     * @return 读取的字符，如果失败则返回 EOF。
     */
    virtual int get() = 0;

    /**
     * @brief 忽略指定数量的字符。
     * @param n 要忽略的字符数，默认为 1。
     * @param delim 停止字符，默认为 EOF。
     * @return 引用自身。
     */
    virtual FileReader& ignore(std::streamsize n = 1, int delim = EOF) = 0;

    /**
     * @brief 读取一些字符到缓冲区。
     * @param s 缓冲区。
     * @param n 缓冲区大小。
     * @return 读取的字节数。
     */
    virtual std::streamsize readsome(char* s, std::streamsize n) = 0;

    /**
     * @brief 将字符放回输入流。
     * @param ch 要放回的字符。
     * @return 引用自身。
     */
    virtual FileReader& putback(char ch) = 0;

    /**
     * @brief 取消获取最后一个字符。
     * @return 引用自身。
     */
    virtual FileReader& unget() = 0;

    // 类型转换操作符，方便使用
    virtual operator bool() const = 0;
};

} // namespace furry_toy

#endif // __FURRY_TOY_FILE_READER_H__