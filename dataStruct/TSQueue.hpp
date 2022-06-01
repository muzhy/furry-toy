/********************************************************
 * TSQueue thread safe queue 线程安全队列 
 * *****************************************************/

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#ifndef __FURRY_TOY_THREAD_SAFE_QUEUE_H__
#define __FURRY_TOY_THREAD_SAFE_QUEUE_H__

namespace furry_toy
{
    template <typename T>
    class TSQueue
    {
    public:
        TSQueue() {}
        TSQueue(const TSQueue& otherQueue)
        {
            std::lock_guard<std::mutex> lock(otherQueue.m_dataMut);
            m_data = otherQueue.m_data;
        }
        // 禁止赋值操作
        TSQueue& operator=(const TSQueue&) = delete;

        void push(T value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            m_data.push(value);
            m_dataCond.notify_one();
        }
        //立即返回，若队列中没有数据，则返回false或nullptr
        bool try_pop(T& value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            if(m_data.empty())
            {
                return false;
            }
            value = m_data.front();
            m_data.pop();
            return true;
        }  
        std::shared_ptr<T> try_pop()
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            if(m_data.empty())
            {
                return nullptr;
            }
            std::shared_ptr<T> res(std::make_shared<T>(m_data.front()));
            m_data.pop();
            return res;
        }
        //若队列中没有数据，则会阻塞，直到队列中有数据再返回
        void pop(T& value)
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            m_dataCond.wait(lock, [this]{ return !m_data.empty(); });
            value = m_data.front();
            m_data.pop();
        }
        std::shared_ptr<T> pop()
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            m_dataCond.wait(lock, [this]{ return !m_data.empty(); });
            std::shared_ptr<T> res(std::make_shared<T>(m_data.front()));
            m_data.pop();
            return res;
        }
        
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return m_data.empty();
        }
    private:
        std::queue<T> m_data;
        mutable std::mutex m_dataMut;
        std::condition_variable m_dataCond;
    };
}

#endif
