#pragma once
#include "Animation.h"

namespace mc
{
    /// CustomAnimation is an animation class that allows the user to define their own animation logic.
    /// Before starting the animation, SetAnimationFunction() must be called
    /// to set the user defined function that controls the animation logic.
    class CustomAnimation : public Animation
    {
    public:
        CustomAnimation();

        /// Sets the user defined callback that gets executed
        /// on every tick (every milisecond) during the animation.
        void SetAnimationFunction(std::function<void(uint32_t)> fn) { m_UserOnTick = fn; }

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
        std::function<void(uint32_t)> m_UserOnTick;
    };
}