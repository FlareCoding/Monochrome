#include "FadeOutAnimation.h"

namespace mc
{
    FadeOutAnimation::FadeOutAnimation()
    {
        ANIMATION_INSTANCE_CONSTRUCTOR
    }

    void FadeOutAnimation::Animate(
        uint32_t duration,
        std::function<void()> completion_handler
    )
    {
        if (m_Started) return;
        m_Started = true;

        m_FadeStep = (Target->layer.color.alpha - m_TargetAlpha) / (float)duration;
        
        m_CompletionHandler = completion_handler;
        m_Timer.Start(1);
    }

    void FadeOutAnimation::OnTick(uint32_t tick, UITimer* timer)
    {
        Target->layer.color.alpha -= m_FadeStep;

        if (Target->layer.color.alpha <= m_TargetAlpha)
        {
            m_Timer.Stop();
            Target->layer.color.alpha = m_TargetAlpha;
            Target->Visible = false;
            if (m_CompletionHandler)
                m_CompletionHandler();

            DestroyAnimation(this);
        }
    }
}
