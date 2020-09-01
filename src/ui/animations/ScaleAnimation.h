#pragma once
#include "Animation.h"

namespace mc
{
    /// ScaleAnimation is an animation class that allows to gradually resize/scale a UIView over a specified duration.
    /// Before starting the animation, SetTargetSize() must be called
    /// to set the desired final size the UIView should be.
    class ScaleAnimation : public Animation
    {
    public:
        ScaleAnimation();

        /// Sets the final desired size the target UIView should be.
        void SetTargetSize(Size size) { m_TargetSize = size; }

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
        float m_Duration = 0.0f;
        Size m_TargetSize = { 0.0f, 0.0f };
        Size m_ScaleStep = { 0.0f, 0.0f };
    };
}