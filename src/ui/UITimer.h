#pragma once
#include <core/Core.h>
#include <functional>

namespace mc
{
    class UITimer
    {
    public:
        using OnTickCallback_t = std::function<void(uint32_t tick, UITimer* timer)>;

    public:
        UITimer() = default;
        UITimer(OnTickCallback_t fn);
        ~UITimer() = default;

        /// Starts the timer that will call its on-tick callback once per specified interval.
        void Start(uint32_t intervals);

        /// Waits for the specified wait_time and starts the timer.
        void Schedule(uint32_t wait_time, uint32_t intervals);

        /// Stops the timer's loop and exits out of its thread.
        void Stop();

        /// Specified the callback function to call on every tick.
        void SetOnTickFunction(OnTickCallback_t fn);

    private:
        bool m_Running = false;
        OnTickCallback_t m_OnTickCallback = nullptr;
    };
}
