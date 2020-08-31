#pragma once
#include "Animation.h"

namespace mc
{
    class FadeOutAnimation : public Animation
    {
    public:
        FadeOutAnimation();

        void Animate(
            uint32_t duration,
            std::function<void()> completion_handler = nullptr
        ) override;

    private:
        std::function<void()> m_CompletionHandler = nullptr;

        bool m_Started = false;
        void OnTick(uint32_t tick, UITimer* timer) override;

    private:
        float m_TargetAlpha = 0.0f;
        float m_FadeStep = 0.0f;
    };
}
