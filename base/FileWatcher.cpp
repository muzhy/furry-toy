#include "FileWatcher.h"
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <errno.h>
#include <poll.h>

/**
 * @brief 构造函数，初始化成员变量。
 * @param rootPath 根文件夹路径。
 */
FileWatcher::FileWatcher(const std::string& rootPath) : m_rootPath(rootPath), m_fd(-1), m_pipeFd{-1, -1}, m_stopFlag(false) {}

/**
 * @brief 析构函数，关闭文件描述符。
 */
FileWatcher::~FileWatcher() {
    if (m_fd >= 0) {
        close(m_fd);
    }
    if (m_pipeFd[0] >= 0) {
        close(m_pipeFd[0]);
    }
    if (m_pipeFd[1] >= 0) {
        close(m_pipeFd[1]);
    }
}

/**
 * @brief 初始化 inotify 和添加根文件夹监视。
 * @return true 如果成功，false 如果失败。
 */
bool FileWatcher::init() {
    if (pipe(m_pipeFd) < 0) {
        std::cerr << "Failed to create pipe: " << strerror(errno) << std::endl;
        return false;
    }
    m_fd = inotify_init();
    if (m_fd < 0) {
        std::cerr << "Failed to initialize inotify: " << strerror(errno) << std::endl;
        close(m_pipeFd[0]);
        close(m_pipeFd[1]);
        m_pipeFd[0] = m_pipeFd[1] = -1;
        return false;
    }
    int wd = inotify_add_watch(m_fd, m_rootPath.c_str(), IN_CLOSE_WRITE | IN_CREATE | IN_DELETE);
    if (wd < 0) {
        std::cerr << "Failed to add watch for " << m_rootPath << ": " << strerror(errno) << std::endl;
        close(m_fd);
        m_fd = -1;
        close(m_pipeFd[0]);
        close(m_pipeFd[1]);
        m_pipeFd[0] = m_pipeFd[1] = -1;
        return false;
    }
    m_watchToPath[wd] = m_rootPath;
    return true;
}

/**
 * @brief 设置关闭写入事件的回调。
 * @param callback 回调函数。
 */
void FileWatcher::setOnCloseWrite(std::function<void(const std::string&)> callback) {
    m_onCloseWrite = callback;
}

/**
 * @brief 设置创建事件的回调。
 * @param callback 回调函数。
 */
void FileWatcher::setOnCreate(std::function<void(const std::string&)> callback) {
    m_onCreate = callback;
}

/**
 * @brief 设置删除事件的回调。
 * @param callback 回调函数。
 */
void FileWatcher::setOnDelete(std::function<void(const std::string&)> callback) {
    m_onDelete = callback;
}

/**
 * @brief 启动监控循环，读取和处理事件，直到停止。
 */
void FileWatcher::start() {
    const size_t BUF_SIZE = 4096;  // 更大的缓冲区以减少溢出风险
    char buf[BUF_SIZE];
    // 监听两个事件，一个是inotify事件，一个是停止信号
    struct pollfd fds[2];
    fds[0].fd = m_fd;
    fds[0].events = POLLIN;
    fds[1].fd = m_pipeFd[0];
    fds[1].events = POLLIN;

    while (!m_stopFlag.load()) {
        int ret = poll(fds, 2, -1);  // 无限等待
        if (ret < 0) {
            if (errno == EINTR) continue;
            std::cerr << "Poll error: " << strerror(errno) << std::endl;
            break;
        }
        if (ret == 0) continue;  // 超时，但我们用-1

        if (fds[0].revents & POLLIN) {
            // inotify事件
            ssize_t len = read(m_fd, buf, sizeof(buf));
            if (len < 0) {
                if (errno == EINTR) continue;
                std::cerr << "Read error: " << strerror(errno) << std::endl;
                break;
            }
            size_t i = 0;
            while (i < len) {
                struct inotify_event* event = (struct inotify_event*)&buf[i];
                if (i + sizeof(struct inotify_event) > len) {
                    std::cerr << "Incomplete event structure, skipping" << std::endl;
                    break;
                }
                size_t eventSize = sizeof(struct inotify_event) + event->len;
                if (i + eventSize > len) {
                    std::cerr << "Event size exceeds buffer, skipping" << std::endl;
                    break;
                }
                processEvent(event);
                i += eventSize;
            }
        }
        if (fds[1].revents & POLLIN) {
            // 停止信号，读取并忽略
            char dummy;
            read(m_pipeFd[0], &dummy, 1);
        }
    }
    // 清理所有监视
    for (auto& p : m_watchToPath) {
        inotify_rm_watch(m_fd, p.first);
    }
    m_watchToPath.clear();
    if (m_fd >= 0) {
        close(m_fd);
        m_fd = -1;
    }
}

/**
 * @brief 停止监控，设置停止标志。
 */
void FileWatcher::stop() {
    m_stopFlag.store(true);
    // 写入pipe来唤醒poll
    char dummy = 'x';
    write(m_pipeFd[1], &dummy, 1);
}

/**
 * @brief 处理单个事件，调用相应回调。
 * @param event 事件结构体。
 */
void FileWatcher::processEvent(struct inotify_event* event) {
    auto it = m_watchToPath.find(event->wd);
    if (it == m_watchToPath.end()) return;
    std::string parentPath = it->second;
    std::string fullPath = parentPath + "/" + event->name;
    if (event->mask & IN_CLOSE_WRITE) {
        if (m_onCloseWrite) m_onCloseWrite(fullPath);
    } else if (event->mask & IN_CREATE) {
        if (m_onCreate) m_onCreate(fullPath);
        // 检查是否为目录，如果是则递归添加监视
        struct stat s_buf;
        if (stat(fullPath.c_str(), &s_buf) == 0 && S_ISDIR(s_buf.st_mode)) {
            int wd = inotify_add_watch(m_fd, fullPath.c_str(), IN_CLOSE_WRITE | IN_CREATE | IN_DELETE);
            if (wd >= 0) {
                m_watchToPath[wd] = fullPath;
            } else {
                std::cerr << "Failed to add watch for " << fullPath << ": " << strerror(errno) << std::endl;
            }
        }
    } else if (event->mask & IN_DELETE) {
        if (m_onDelete) m_onDelete(fullPath);
    } else if (event->mask & (IN_DELETE_SELF | IN_MOVE_SELF | IN_IGNORED)) {
        m_watchToPath.erase(event->wd);
        inotify_rm_watch(m_fd, event->wd);
    }
}