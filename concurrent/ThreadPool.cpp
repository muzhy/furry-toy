#include "ThreadPool.h"

#include <exception>

namespace furry_toy
{

bool ThreadPool::init(const int nMaxThreadCount, const int nMinThreadCount, const int nIdleTimeout)
{
    std::lock_guard<std::mutex> lock(m_mut);
    // 禁止重复初始化
    if(m_bIsInited)
    {
        return false;
    }

    m_bIsInited = true;
    m_nMaxThreadCount = nMaxThreadCount;
    m_nMinThreadCount = nMinThreadCount;
    m_nIdleTimeout = m_nIdleTimeout;

    return true;
}

bool ThreadPool::isNeedExit()
{
    std::lock_guard<std::mutex> lock(m_mut);
    return !m_bIsStop;
}

bool ThreadPool::addJob(Job&& job)
{
    m_jobQueue.push(std::move(job));
    return false;
}

std::shared_ptr<ThreadPool::Job> ThreadPool::getJob(int nTimeout)
{
    if(nTimeout == ThreadPool::WAIT_JOB_TIMEOUT_UNLIMIT)
    {
        return m_jobQueue.pop();
    }
    else
    {
        return m_jobQueue.try_pop(nTimeout);
    }
}

void ThreadPool::SubThread::run()
{
    while(!m_parent.isNeedExit())
    {
        auto jobPtr = m_parent.getJob(m_parent.m_nIdleTimeout);
        if(!jobPtr && m_parent.m_nCurrThreadCount > m_parent.m_nMinThreadCount)
        {
            // 销毁自己
            return ;
        }
    }
}

bool ThreadPool::start()
{
    std::lock_guard<std::mutex> lock(m_mut);
    if(m_bIsStarted) { return false; }
    if(!m_bIsInited) { return false; }
    
    m_bIsStarted = true;
    // TODO 创建线程并启动
    return true;
}
    
}