#include "ScaleAnimation.h"

namespace mc
{
    ScaleAnimation::ScaleAnimation()
    {
        ANIMATION_INSTANCE_CONSTRUCTOR
    }

    void ScaleAnimation::Animate(
        uint32_t duration,
        std::function<void()> completion_handler
    )
    {
        if (m_Started) return;
        m_Started = true;
        
        m_ScaleStep.width = (m_TargetSize.width - Target->layer.frame.size.width) / (float)duration;
        m_ScaleStep.height = (m_TargetSize.height - Target->layer.frame.size.height) / (float)duration;
        
        m_CompletionHandler = completion_handler;
        m_Duration = duration;
        m_Timer.Start(1);
    }

    void ScaleAnimation::OnTick(uint32_t tick, UITimer* timer)
    {
        Target->layer.frame.size += m_ScaleStep;
        
        if (tick >= m_Duration)
        {
            m_Timer.Stop();
            if (m_CompletionHandler)
                m_CompletionHandler();

            DestroyAnimation(this);
        }
    }
}

