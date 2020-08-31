#include "TranslationAnimation.h"

namespace mc
{
    TranslationAnimation::TranslationAnimation()
    {
        ANIMATION_INSTANCE_CONSTRUCTOR
    }

    void TranslationAnimation::Animate(
        uint32_t duration,
        std::function<void()> completion_handler
    )
    {
        if (m_Started) return;
        m_Started = true;
        
        m_MovementStep.x = m_TargetDistance.x / (float)duration;
        m_MovementStep.y = m_TargetDistance.y / (float)duration;
        
        m_CompletionHandler = completion_handler;
        m_Duration = duration;
        m_Timer.Start(1);
    }

    void TranslationAnimation::OnTick(uint32_t tick, UITimer* timer)
    {
        Target->layer.frame.position += m_MovementStep;
        
        if (tick >= m_Duration)
        {
            m_Timer.Stop();
            if (m_CompletionHandler)
                m_CompletionHandler();

            DestroyAnimation(this);
        }
    }
}
