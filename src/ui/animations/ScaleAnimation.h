#pragma once
#include "Animation.h"

namespace mc
{
    class ScaleAnimation : public Animation
    {
    public:
        ScaleAnimation();

        void SetTargetSize(Size size) { m_TargetSize = size; }

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
        Size m_TargetSize = { 0.0f, 0.0f };
        Size m_ScaleStep = { 0.0f, 0.0f };
    };
}