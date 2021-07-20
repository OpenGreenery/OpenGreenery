#include <open_greenery/tools/LoopThread.hpp>
#include <cassert>


namespace open_greenery::tools
{

LoopThread::LoopThread(std::function<void()> _func, std::chrono::milliseconds _period)
    :m_stop_flag(true),
    m_periodic_func([_func, _period, this](std::promise<void> && _promise){
        _promise.set_value_at_thread_exit();
        while (!m_stop_flag)
        {
            const auto end = std::chrono::system_clock::now() + _period;
            if (m_stop_flag)
            {
                break;
            }
            _func();
            if (m_stop_flag)
            {
                break;
            }
            std::this_thread::sleep_until(end);
        }
    }),
    m_thr()
{}

LoopThread::~LoopThread()
{
    stop();
    if (m_thr.joinable())
    {
        m_thr.join();
    }
}

FinishFuture LoopThread::start()
{
    std::promise<void> p;
    auto thread_exit = p.get_future();

    assert((!m_thr.joinable() && m_stop_flag) &&
        "Loop thread is still executing. Stop it or check .start() call duplication.");

    m_stop_flag = false;
    m_thr = std::thread(m_periodic_func, std::move(p));

    return thread_exit;
}

void LoopThread::stop()
{
    m_stop_flag = true;
}

}
