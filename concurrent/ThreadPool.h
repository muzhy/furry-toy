#ifndef __FURRY_TOY_THREAD_POOL_H__
#define __FURRY_TOY_THREAD_POOL_H__

#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <optional>
#include <chrono>
#include <optional>

#include "TSQueue.hpp"
#include "base.h"
#include <functional>

namespace furry_toy 
{

class ThreadPool : public UnAssignable
{
public:
    using Job = std::function<void()>;

public: 
    ThreadPool() : m_bIsInited(false), m_bIsStarted(false), m_bIsStop(false), m_nMaxThreadCount(0), m_nMinThreadCount(0), m_nIdleTimeout(std::chrono::seconds(60)) {}
    ~ThreadPool() { stop(); }

    // initialize the thread pool with max/min thread count and idle timeout
    bool init(const int nMaxThreadCount, const int nMinThreadCount, const std::chrono::seconds nIdleTimeout);

    // put job into thread pool's job queue waiting for execution
    bool putJob(const Job& job);

    // stop the thread pool and wait for all threads to exit
    // if jobs are still in the queue, they will not be executed
    // if called multiple times, only the first call is effective
    void stop();

private:
    // get a job from the job queue, wait up to timeout duration 
    // or return nullptr if needed to exit
    std::shared_ptr<Job> waitJob();

private:
    class SubThread : public UnAssignable
    {
    public:
        SubThread(ThreadPool& parent) : m_parent(parent), m_pThread(nullptr) {}

        void start();
        void join();
    private:
        void run();
    private:
        ThreadPool& m_parent;
        std::thread* m_pThread;
    };

private:
    TSQueue<Job> m_jobQueue;   //待处理的任务队列

    std::mutex m_mut;
    bool m_bIsInited = false;
    bool m_bIsStarted = false;
    bool m_bIsStop = false;
    int m_nMaxThreadCount = 0;
    int m_nMinThreadCount = 0;
    std::chrono::seconds m_nIdleTimeout = std::chrono::seconds(60);
};

}
#endif