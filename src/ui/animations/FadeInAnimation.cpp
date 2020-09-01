#include "FadeInAnimation.h"

namespace mc
{
    FadeInAnimation::FadeInAnimation()
    {
        ANIMATION_INSTANCE_CONSTRUCTOR
    }

    void FadeInAnimation::Animate(
        uint32_t duration,
        std::function<void()> completion_handler
    )
    {
        if (m_Started) return;
        m_Started = true;
        
        m_FadeStep = (m_TargetAlpha - Target->layer.color.alpha) / (float)duration;
        Target->Visible = true;
        
        m_CompletionHandler = completion_handler;
        m_Timer.Start(1);
    }

    void FadeInAnimation::OnTick(uint32_t tick, UITimer* timer)
    {
        Target->layer.color.alpha += m_FadeStep;
        
        if (Target->layer.color.alpha >= m_TargetAlpha)
        {
            m_Timer.Stop();
            Target->layer.color.alpha = m_TargetAlpha;
            if (m_CompletionHandler)
                m_CompletionHandler();

            DestroyAnimation(this);
        }
    }
}
