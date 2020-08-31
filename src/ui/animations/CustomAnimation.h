#pragma once
#include "Animation.h"

namespace mc
{
    class CustomAnimation : public Animation
    {
    public:
        CustomAnimation();

        void SetAnimationFunction(std::function<void(uint32_t)> fn) { m_UserOnTick = fn; }

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
        std::function<void(uint32_t)> m_UserOnTick;
    };
}