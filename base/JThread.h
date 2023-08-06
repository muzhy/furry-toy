#ifndef __FURRY_TOY_BASE_JOINNING_THREAD_H__
#define __FURRY_TOY_BASE_JOINNING_THREAD_H__

#include <thread>
#include <utility>

namespace furry_toy
{

// JThread : Joinning Thread
// std::thread's proxy, call join() when JThread destruct
class JThread
{
private:
    std::thread m_thread;
public:
    JThread() noexcept = default;
    template<typename Callable, typename ... Args>
    explicit JThread(Callable&& func, Args&& ... args) :
        m_thread(std::forward<Callable>(func), std::forward<Args>(args) ...)
    {}
    explicit JThread(std::thread t) noexcept : 
        m_thread(std::move(t))
    {}
    JThread(JThread&& other) noexcept :
        m_thread(std::move(other.m_thread))
    {}
    JThread& operator=(JThread&& other) noexcept 
    {
        if(joinable())
        {
            join();
        }
        m_thread = std::move(other.m_thread);
        return *this;
    }
    JThread& operator=(std::thread&& other) noexcept
    {
        if(joinable())
        {
            join();
        }
        m_thread = std::move(other);
        return *this;
    }

    ~JThread() noexcept 
    {
        if(joinable())
        {
            join();
        }
    }

    void swap(JThread&& other) noexcept
    {
        m_thread.swap(other.m_thread);
    }

    // proxy std::thread functions
    bool joinable() const noexcept {
        return m_thread.joinable();
    }
    void join() 
    {
        m_thread.join();
    }
    void detach()
    {
        m_thread.detach();
    }
    
    std::thread::id get_id() const noexcept 
    {
        return m_thread.get_id();
    }

    std::thread& as_thread() noexcept{
        return m_thread;
    }
    const std::thread& as_thread() const noexcept
    {
        return m_thread;
    }

    std::thread::native_handle_type native_hadle()
    {
        return m_thread.native_handle();
    }
};

} // namespace furry_toy



#endif //__FURRY_TOY_BASE_JOINNING_THREAD_H__