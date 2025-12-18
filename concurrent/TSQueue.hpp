#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <optional>

#include "base.h"

#ifndef __FURRY_TOY_THREAD_SAFE_QUEUE_H__
#define __FURRY_TOY_THREAD_SAFE_QUEUE_H__

namespace furry_toy
{
    template <typename T>
    class TSQueue : public UnAssignable
    {
    public:
        TSQueue() : m_data(), m_dataMut(), m_dataCond(), m_destory(false) {}
        
        ~TSQueue()
        {
            {
                std::lock_guard<std::mutex> lock(m_dataMut);
                m_destory = true;
            }
            m_dataCond.notify_all();
        }
        
        template <typename U>
        void push(U&& value)
        {
            std::shared_ptr<T> value_ptr(std::make_shared<T>(std::forward<U>(value)));
            std::lock_guard<std::mutex> lock(m_dataMut);
            m_data.push(value_ptr);
            m_dataCond.notify_one();
        }

        //立即返回，若队列中没有数据，则返回std::nullopt
        std::optional<std::shared_ptr<T>> try_pop()
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return popPtr();
        }
        
        // 等待数据，超时还没有数据时返回std::nullopt
        template<typename Rep, typename Period>
        std::optional<std::shared_ptr<T>> try_pop(std::chrono::duration<Rep, Period> timeout)
        {
            std::unique_lock<std::mutex> lock(m_dataMut);
            if (m_dataCond.wait_for(lock, timeout, [this]{
                return !m_data.empty() || m_destory;
            }))
            {
                return popPtr();
            }
            else
            {
                return std::nullopt;
            }
        }

        //若队列中没有数据，则会阻塞，直到队列中有数据或队列被销毁时再返回
        // 若队列表销毁且为空，返回std::nullopt
        std::optional<std::shared_ptr<T>> pop()
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
        
        size_t size() const
        {
            std::lock_guard<std::mutex> lock(m_dataMut);
            return m_data.size();
        }

    private:
        inline std::shared_ptr<T> popPtr()
        {
            if(m_data.empty())
            {
                return nullptr;
            }
            std::shared_ptr<T> res = m_data.front();
            m_data.pop();
            return res;
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
