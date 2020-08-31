#include "UITimer.h"

namespace mc
{
    UITimer::UITimer(OnTickCallback_t fn)
        : m_OnTickCallback(fn)
    {
    }

    void UITimer::Start(uint32_t intervals)
    {
        std::thread timer_thread([this, intervals]() {
            m_Running = true;
            uint32_t tick = 0;
            while (m_Running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(intervals));
                tick++;
                
                if (m_OnTickCallback)
                    m_OnTickCallback(tick, this);
            }
        });
        timer_thread.detach();
    }

    void UITimer::Schedule(uint32_t wait_time, uint32_t intervals)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
        Start(intervals);
    }

    void UITimer::Stop()
    {
        m_Running = false;
    }

    void UITimer::SetOnTickFunction(OnTickCallback_t fn)
    {
        m_OnTickCallback = fn;
    }
}
