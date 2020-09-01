#pragma once
#include "Animation.h"

namespace mc
{
    /// TranslationAnimation is an animation class that allows to gradually displace/translate a UIView over a specified duration.
    /// Before starting the animation, SetTargetTranslation() must be called
    /// to set the desired distance the UIView should be moved.
    class TranslationAnimation : public Animation
    {
    public:
        TranslationAnimation();

         /// Sets the desired distance the target UIView should be moved.
        void SetTargetTranslation(Size distance) { m_TargetDistance = distance; }

        /// Starts the animation that will be performed in a separate background thread.
        /// @param duration Time in miliseconds the animation is going to run for.
        /// @param completion_handler Optional user-defined function that will run
        /// upon completion of the animation.
        void Animate(
            uint32_t duration,
            std::function<void()> completion_handler = nullptr
        ) override;

    private:
        std::function<void()> m_CompletionHandler = nullptr;

        bool m_Started = false;
        void OnTick(uint32_t tick, UITimer* timer) override;

    private:
        uint32_t m_Duration = 0.0f;
        Size m_TargetDistance = { 0.0f, 0.0f };
        Size m_MovementStep = { 0.0f, 0.0f };
    };
}