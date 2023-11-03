#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "base.h"

#ifndef __FURRY_TOY_THREAD_SAFE_QUEUE_H__
#define __FURRY_TOY_THREAD_SAFE_QUEUE_H__

namespace furry_toy
{
    template <typename T>
    class TSStack 
    {
    private:
        std::stack<T> m_data;
        mutable std::mutex m_mut;
    public:
        TSStack() : m_data(std::stack<T>()) {}
        TSStack(const TSStack& other)
        {
            std::lock_guard<std::mutex> lock(other.m_mut);
            m_data = other.m_data;
        }
        TSStack& operator=(const TSStack&) = delete;
        
        void push(T value)
        {
            std::lock_guard<std::mutex> lock(m_mut);
            m_data.push(value);
        }
        void pop(T& value)
        {
            std::lock_guard<std::mutex> lock(m_mut);
            if(m_data.empty())
            {
                throw "empty stack";
            }
            value = m_data.top();
            m_data.pop();
        }

        std::shared_ptr<T> pop()
        {
            std::lock_guard<std::mutex> lock(m_mut);
            if(m_data.empty())
            {
                throw "empty stack";
            }
            std::shared_ptr<T> const res(std::make_shared<T>(m_data.top()));
            m_data.pop();
            return res;
        }
        bool empty() const 
        {
            std::lock_guard<std::mutex> lock(m_mut);
            return m_data.empty();
        }
    };

    /********************************************************
    * TSQueue thread safe queue 线程安全队列 
    * *****************************************************/
    template <typename T>
    class TSQueue : public UnAssignable
    {
    public:
        typedef std::unique_ptr<T> UniPtrType;
        typedef std::shared_ptr<T> SharedPtrType;
    public:
        TSQueue() : m_data(), m_dataMut(), m_dataCond(), m_destory(false) {}
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
        
        void push(const T& value)
        {
            std::shared_ptr<T> value_ptr(std::make_shared<T>(std::move(value)));
            std::lock_guard<std::mutex> lock(m_dataMut);
            m_data.push(value_ptr);
            m_dataCond.notify_one();
        }
        // 适用于右值引用的情况
        void push(T&& value)
        {
            std::shared_ptr<T> value_ptr(std::make_shared<T>(std::move(value)));
            std::lock_guard<std::mutex> lock(m_dataMut);
            // m_data.push(std::move(value));
            m_data.push(value_ptr);
            m_dataCond.notify_one();
        }

        //立即返回，若队列中没有数据，则返回false或nullptr
        bool try_pop(T& value)
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return popValue(value);
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
                return popPtr();
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

        std::unique_ptr<T> popUniquePtr()
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            m_dataCond.wait(lock, [this]{ return !m_data.empty() || m_destory; });
            return popUniPtr();
        }
        
        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return m_data.empty();
        }

    private:
        inline std::unique_ptr<T> popUniPtr()
        {
            if(m_data.empty())
            {
                return nullptr;
            }
            auto res = std::make_unique<T>(std::move(m_data.front()));
            m_data.pop();
            return res;
        }

        inline std::shared_ptr<T> popPtr()
        {
            if(m_data.empty())
            {
                return nullptr;
            }
            std::shared_ptr<T> res = m_data.front();
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
            value = std::move(*m_data.front());
            m_data.pop();
            return true;
        }

    private:
        // std::queue<T> m_data;
        // 直接存储共享指针，可以在push中加锁之前创建对象，而不需要pop中创建，缩小锁的范围
        // 同时，避免再pop中创建shared_ptr<T>对象的时候发生异常导致没有线程处理数据。
        std::queue<std::shared_ptr<T>> m_data;
        mutable std::mutex m_dataMut;
        std::condition_variable m_dataCond;
        bool m_destory;
    };
}

#endif
