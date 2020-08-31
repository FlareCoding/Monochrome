#include "CustomAnimation.h"

namespace mc
{
    CustomAnimation::CustomAnimation()
    {
        ANIMATION_INSTANCE_CONSTRUCTOR
    }

    void CustomAnimation::Animate(
        uint32_t duration,
        std::function<void()> completion_handler
    )
    {
        if (m_Started) return;
        m_Started = true;
        
        m_CompletionHandler = completion_handler;
        m_Duration = duration;
        m_Timer.Start(1);
    }

    void CustomAnimation::OnTick(uint32_t tick, UITimer* timer)
    {
        if (m_UserOnTick)
            m_UserOnTick(tick);
        
        if (tick >= m_Duration)
        {
            m_Timer.Stop();
            if (m_CompletionHandler)
                m_CompletionHandler();

            DestroyAnimation(this);
        }
    }
}


