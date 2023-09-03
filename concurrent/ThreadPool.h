#ifndef __FURRY_TOY_THREAD_POOL_H__
#define __FURRY_TOY_THREAD_POOL_H__

#include <memory>
#include <mutex>
#include <thread>

#include "TSQueue.hpp"
#include "base.h"

namespace furry_toy 
{

/*************************************************************
 * ThreadPool 线程池，用于接收任务，并分配给不同线程去执行
 * ThreadPool对象不可赋值，不可拷贝
 * 在创建ThreadPool对象后，需要调用init()进行初始化，不可重复初始化
 * 初始化可设置线程池内线程数量的上下限，以及线程空闲被回收的时间
 * 参数可不填，不填时将使用默认值
 * 
 * 要放到ThreadPool中的任务必须是ThreadPool::Job的子类
 * 通过调用ThreadPool::addJob()将要执行的任务添加到线程池中等待调度
***************************************************************/
class ThreadPool : public UnAssignable
{
public:
    /*****************************************************
     * Job 一个任务的抽象表示，该类是一个抽象基类，
     * 所有要放到线程池中执行的任务都需要是Job的子类
     * 通过调用ThreadPool::addJob将任务添加到线程池后，任务
     * 将由线程池进行调度执行
     * Job的子类必须实现Job::run()函数，当任务被分配到线程进行
     * 执行时，线程调用Job::run()执行任务
    *******************************************************/
    class Job
    {
    public:
        Job() {}
        virtual ~Job() {}

        virtual void run();       
    };
public:
    ThreadPool() : m_bIsInited(false), m_bIsStarted(false), m_bIsStop(false),
        m_nMaxThreadCount(0), m_nMinThreadCount(0), m_nCurrThreadCount(0), m_nIdleTimeout(0),
        m_jobQueue() {}
    ~ThreadPool();
    
    bool init(const int nMaxThreadCount = 16, 
        const int nMinThreadCount = 8, 
        const int nIdleTimeout = 300);

    bool start();
    bool stop();
    
    bool addJob(Job&& JobPtr);
private:
    /*************************************************
     * ThreadPool内部的子线程类
     * 需要获取Job并执行，在完成任务后决定是否退出
     * 在标准库的std::thread类上进行定制化
     * 内部包含一个std::thread的指针，不采用继承的方式
    ***************************************************/
    class SubThread
    {
    public:
        SubThread(ThreadPool& parent) : m_pThread(nullptr), m_parent(parent) { }
        void start();
    private:
        void run();
        bool isNeedExit();

        std::thread* m_pThread;
        ThreadPool& m_parent;
    };

private:
    bool isNeedExit();
    bool isNeedDestorySubThread();
    void afterDestorySubThread();

    /******************************************************
     * getJob 获取待处理的
     * @param nTimeout 愿意阻塞的时间，单位为毫秒
     *  当nTimeout为ThreadPool::WAIT_JOB_TIMEOUT_UNLIMIT时
     *  表示一直等待下去，直到获取到数据或队列被销毁
     * @return std::shared_ptr<Job> 
     *  获取不到时返回nullptr，否则返回Job的shared_ptr
    */
    std::shared_ptr<Job> getJob(int nTimeout = ThreadPool::WAIT_JOB_TIMEOUT_UNLIMIT);
private:
    static const int WAIT_JOB_TIMEOUT_UNLIMIT = -1;

    std::mutex m_mut;
    bool m_bIsInited;
    bool m_bIsStarted;         
    bool m_bIsStop;                 
    int m_nMaxThreadCount;                      //线程池可以同时拥有的最大线程数
    int m_nMinThreadCount;                      //线程池可以同时拥有的最小线程数
    int m_nCurrThreadCount;                     //当前线程池拥有的线程数
    int m_nIdleTimeout;                         //线程空闲多长时间可以销毁，

    TSQueue<Job> m_jobQueue;   //待处理的任务队列
};

}

#endif