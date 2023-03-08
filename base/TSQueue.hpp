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
        TSQueue() : m_data(), m_dataMut(), m_dataCond() {}
        TSQueue(const TSQueue& otherQueue) 
        {
            std::lock_guard<std::mutex> lock(otherQueue.m_dataMut);
            m_data = otherQueue.m_data;
        }
        ~TSQueue()
        {
            {
                std::lock_guard<std::mutex> lock(m_dataMut);
                m_destory = true;
            }
            m_dataCond.notify_all();
        }
        // 禁止赋值操作
        TSQueue& operator=(const TSQueue&) = delete;

        void push(const T& value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            m_data.push(value);
            m_dataCond.notify_one();
        }
        // 适用于右值引用的情况
        void push(T&& value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            m_data.push(std::move(value));
            m_dataCond.notify_one();
        }

        //立即返回，若队列中没有数据，则返回false或nullptr
        bool try_pop(T& value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return popValue();
        }  
        
        std::shared_ptr<T> try_pop()
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            if(m_data.empty())
            {
                return nullptr;
            }
            return popPtr();
        }
        // 等到数据，超时还没有数据时返回
        bool try_pop(T&value, unsigned int timeout)
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            if(m_dataCond.wait_for(lock, std::chrono::milliseconds(timeout), [this]{
                return m_data.empty() == false || m_destory;
            }))            
            {
                return popValue();
            }
            else
            {
                return false;
            }
        }
        std::shared_ptr<T> try_pop(unsigned int timeout)
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            if(m_dataCond.wait_for(lock, std::chrono::milliseconds(timeout), [this]{
                return m_data.empty() == false || m_destory;
            }))
            {
                popPtr();
            }
            else
            {
                return nullptr;
            }
        }

        //若队列中没有数据，则会阻塞，直到队列中有数据或队列被销毁时再返回
        // 若队列表销毁，此时pop返回false
        bool pop(T& value)
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            m_dataCond.wait(lock, [this]{ return !m_data.empty() || m_destory; });
            return popValue(value);
        }
        
        std::shared_ptr<T> pop()
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            m_dataCond.wait(lock, [this]{ return !m_data.empty() || m_destory; });
            return popPtr();
        }
        
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return m_data.empty();
        }

    private:
        inline std::shared_ptr<T> popPtr()
        {
            if(m_data.empty())
            {
                return nullptr;
            }
            std::shared_ptr<T> res(std::make_shared<T>(std::move(m_data.front())));
            // auto res = std::move(m_data.front());
            m_data.pop();
            return res;
        }

        inline bool popValue(T& value)
        {
            if(m_data.empty())
            {
                return false;
            }
            value = std::move(m_data.front());
            m_data.pop();
        }

    private:
        std::queue<T> m_data;
        mutable std::mutex m_dataMut;
        std::condition_variable m_dataCond;
        bool m_destory;
    };
}

#endif
