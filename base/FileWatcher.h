#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <string>
#include <functional>
#include <map>
#include <atomic>

/**
 * @brief FileWatcher 类用于监控文件系统事件，支持递归监控目录。
 * 
 * 该类封装了 inotify API，提供初始化、启动监控、停止监控和事件回调功能。
 */
class FileWatcher {
public:
    /**
     * @brief 构造函数，设置要监控的根文件夹路径。
     * @param rootPath 根文件夹路径。
     */
    FileWatcher(const std::string& rootPath);

    /**
     * @brief 析构函数，清理资源。
     */
    ~FileWatcher();

    /**
     * @brief 初始化 inotify 资源和根文件夹监视。
     * @return true 如果初始化成功，false 否则。
     */
    bool init();

    /**
     * @brief 设置 IN_CLOSE_WRITE 事件的回调函数。
     * @param callback 回调函数，参数为文件路径。
     */
    void setOnCloseWrite(std::function<void(const std::string&)> callback);

    /**
     * @brief 设置 IN_CREATE 事件的回调函数。
     * @param callback 回调函数，参数为文件路径。
     */
    void setOnCreate(std::function<void(const std::string&)> callback);

    /**
     * @brief 设置 IN_DELETE 事件的回调函数。
     * @param callback 回调函数，参数为文件路径。
     */
    void setOnDelete(std::function<void(const std::string&)> callback);

    /**
     * @brief 启动监控循环，阻塞直到停止信号被设置。
     */
    void start();

    /**
     * @brief 停止监控循环，设置停止信号。
     */
    void stop();

private:
    int m_fd;  ///< inotify 文件描述符。
    int m_pipeFd[2];  ///< 用于停止信号的管道。
    std::string m_rootPath;  ///< 根文件夹路径。
    std::map<int, std::string> m_watchToPath;  ///< 映射监视描述符到路径。
    std::function<void(const std::string&)> m_onCloseWrite;  ///< 关闭写入事件回调。
    std::function<void(const std::string&)> m_onCreate;  ///< 创建事件回调。
    std::function<void(const std::string&)> m_onDelete;  ///< 删除事件回调。
    std::atomic<bool> m_stopFlag;  ///< 停止标志。

    /**
     * @brief 处理单个 inotify 事件。
     * @param event 事件结构体指针。
     */
    void processEvent(struct inotify_event* event);
};

#endif