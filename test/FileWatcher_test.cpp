#include <gtest/gtest.h>
#include "../base/FileWatcher.h"
#include <thread>
#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;

/**
 * @brief FileWatcher 单元测试。
 */
class FileWatcherTest : public ::testing::Test {
protected:
    std::string testDir;

    void SetUp() override {
        testDir = "/tmp/filewatcher_test";
        fs::create_directories(testDir);
    }

    void TearDown() override {
        fs::remove_all(testDir);
    }
};

/**
 * @brief 测试初始化成功。
 */
TEST_F(FileWatcherTest, InitSuccess) {
    FileWatcher watcher(testDir);
    EXPECT_TRUE(watcher.init());
}

/**
 * @brief 测试初始化失败（无效路径）。
 */
TEST_F(FileWatcherTest, InitFailure) {
    FileWatcher watcher("/invalid/path");
    EXPECT_FALSE(watcher.init());
}

/**
 * @brief 测试创建文件事件回调。
 */
TEST_F(FileWatcherTest, CreateFileEvent) {
    FileWatcher watcher(testDir);
    ASSERT_TRUE(watcher.init());

    bool eventTriggered = false;
    std::string triggeredPath;

    watcher.setOnCreate([&](const std::string& path) {
        eventTriggered = true;
        triggeredPath = path;
    });

    // 启动监控线程
    std::thread watcherThread([&]() {
        watcher.start();
    });

    // 等待一小段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 创建文件
    std::string filePath = testDir + "/testfile.txt";
    std::ofstream file(filePath);
    file.close();

    // 等待事件处理
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // 停止监控
    watcher.stop();
    watcherThread.join();

    EXPECT_TRUE(eventTriggered);
    EXPECT_EQ(triggeredPath, filePath);
}

/**
 * @brief 测试删除文件事件回调。
 */
TEST_F(FileWatcherTest, DeleteFileEvent) {
    // 先创建文件
    std::string filePath = testDir + "/testfile.txt";
    std::ofstream file(filePath);
    file.close();

    FileWatcher watcher(testDir);
    ASSERT_TRUE(watcher.init());

    bool eventTriggered = false;
    std::string triggeredPath;

    watcher.setOnDelete([&](const std::string& path) {
        eventTriggered = true;
        triggeredPath = path;
    });

    // 启动监控线程
    std::thread watcherThread([&]() {
        watcher.start();
    });

    // 等待一小段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 删除文件
    fs::remove(filePath);

    // 等待事件处理
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // 停止监控
    watcher.stop();
    watcherThread.join();

    EXPECT_TRUE(eventTriggered);
    EXPECT_EQ(triggeredPath, filePath);
}

/**
 * @brief 测试关闭写入事件回调。
 */
TEST_F(FileWatcherTest, CloseWriteEvent) {
    FileWatcher watcher(testDir);
    ASSERT_TRUE(watcher.init());

    bool eventTriggered = false;
    std::string triggeredPath;

    watcher.setOnCloseWrite([&](const std::string& path) {
        eventTriggered = true;
        triggeredPath = path;
    });

    // 启动监控线程
    std::thread watcherThread([&]() {
        watcher.start();
    });

    // 等待一小段时间
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 创建并写入文件
    std::string filePath = testDir + "/testfile.txt";
    {
        std::ofstream file(filePath);
        file << "test content";
    }  // 文件关闭时触发 IN_CLOSE_WRITE

    // 等待事件处理
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // 停止监控
    watcher.stop();
    watcherThread.join();

    EXPECT_TRUE(eventTriggered);
    EXPECT_EQ(triggeredPath, filePath);
}