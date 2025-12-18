#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include <atomic>
#include "base/FileWatcher.h"

// 全局停止标志，用于信号处理
std::atomic<bool> globalStop(false);

void signalHandler(int signum) {
    globalStop.store(true);
}

int main(int argc, char *argv[]) {
    // 设置信号处理
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::string rootPath = "/home/zhychen/tmp";  // 可以从 argv 获取
    if (argc > 1) {
        rootPath = argv[1];
    }

    FileWatcher watcher(rootPath);
    if (!watcher.init()) {
        std::cerr << "Failed to initialize FileWatcher" << std::endl;
        return -1;
    }

    // 设置回调函数
    watcher.setOnCloseWrite([](const std::string& path) {
        std::cout << path << " closed after write" << std::endl;
    });
    watcher.setOnCreate([](const std::string& path) {
        std::cout << path << " created" << std::endl;
    });
    watcher.setOnDelete([](const std::string& path) {
        std::cout << path << " deleted" << std::endl;
    });

    // 启动监控线程
    std::thread watcherThread([&]() {
        watcher.start();
    });

    // 主线程等待停止信号
    while (!globalStop.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 停止监控
    watcher.stop();
    watcherThread.join();

    std::cout << "FileWatcher stopped" << std::endl;
    return 0;
}
