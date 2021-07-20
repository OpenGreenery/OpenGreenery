#ifndef LOOP_THREAD_HPP
#define LOOP_THREAD_HPP

#include <atomic>
#include <chrono>
#include <functional>
#include <future>
#include <thread>

namespace open_greenery::tools
{

using FinishFuture = std::future<void>;

class LoopThread
{
public:
    LoopThread(std::function<void()> _func, std::chrono::milliseconds _period);
    ~LoopThread();
    FinishFuture start();
    void stop();

private:
    std::atomic_bool m_stop_flag;
    std::function<void(std::promise<void> &&)> m_periodic_func;
    std::thread m_thr;
};

}

#endif //LOOP_THREAD_HPP
