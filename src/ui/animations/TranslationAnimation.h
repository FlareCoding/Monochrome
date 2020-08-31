#pragma once
#include "Animation.h"

namespace mc
{
    class TranslationAnimation : public Animation
    {
    public:
        TranslationAnimation();

        void SetTargetTranslation(Size distance) { m_TargetDistance = distance; }

        void Animate(
            uint32_t duration,
            std::function<void()> completion_handler = nullptr
        ) override;

    private:
        std::function<void()> m_CompletionHandler = nullptr;

        bool m_Started = false;
        void OnTick(uint32_t tick, UITimer* timer) override;

    private:
        float m_Duration = 0.0f;
        Size m_TargetDistance = { 0.0f, 0.0f };
        Size m_MovementStep = { 0.0f, 0.0f };
    };
}