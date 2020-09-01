#pragma once
#include "Animation.h"

namespace mc
{
    /// FadeOutAnimation is an animation class that allows to gradually fade a UIView in over a specified duration.
    /// The animation is performed by decreasing the alpha value of the view's layer's color on every tick. 
    /// *Remarks: At the end of the animation, it will set the view's Visible flag to false.
    class FadeOutAnimation : public Animation
    {
    public:
        FadeOutAnimation();

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
        float m_TargetAlpha = 0.0f;
        float m_FadeStep = 0.0f;
    };
}
