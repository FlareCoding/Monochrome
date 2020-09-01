#pragma once
#include "Animation.h"

namespace mc
{
    /// FadeInAnimation is an animation class that allows to gradually fade a UIView out over a specified duration.
    /// The animation is performed by decreasing the alpha value of the view's layer's color on every tick.
    /// *Remarks: If the target view initially has the Visible flag set to false, the animation will set it to true.
    class FadeInAnimation : public Animation
    {
    public:
        FadeInAnimation();

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
        float m_TargetAlpha = 1.0f;
        float m_FadeStep = 0.0f;
    };
}
