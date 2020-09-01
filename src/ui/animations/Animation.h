#pragma once
#include <ui/UITimer.h>
#include <ui/UIView.h>

namespace mc
{
    /// Specifies the type of animation.
    enum class AnimationType
    {
        /// Reduces the alpha value of the color of the view until it reaches 0.
        FadeOut,

        /// Increases the alpha value of the color of the view until it reaches 1.
        FadeIn,

        /// Translates the view by a certain amount in pixels.
        Translation,

        /// Resizes the view by a specifies amount in pixels.
        Scale,

        /// Performs a user specified callback over a duration.
        Custom
    };

/// Sets the OnTick member function to be the tick callback for the timer.
#define ANIMATION_INSTANCE_CONSTRUCTOR \
        m_Timer.SetOnTickFunction([this](uint32_t tick, UITimer* timer) { OnTick(tick, timer); });

/// Performs a dynamic pointer cast
#define AnimCast(type, anim) std::dynamic_pointer_cast<type>(anim)

    /// Base class for all framework animations.
    class Animation
    {
    public:
        /// Creates an instance of an appropriate animation class depending on the specified type.
        /// @param type Specifies the type of the animation to be created.
        /// @param target Specifies the target UIView on which specified animation will perform.
        /// @note If the animation type is Custom, target parameter can be left blank.
        /// @returns Ref to the base animation class that can be casted to an appropriate
        /// subclass using AnimCast macro.
        static Ref<Animation> Create(AnimationType type, UIView* target);

        virtual ~Animation() = default;

        /// Starts the animation that will be performed in a separate background thread.
        /// @param duration Time in miliseconds the animation is going to run for.
        /// @param completion_handler Optional user-defined function that will run
        /// upon completion of the animation.
        virtual void Animate(
            uint32_t duration,
            std::function<void()> completion_handler = nullptr
        ) = 0;

    protected:
        UITimer m_Timer;
        virtual void OnTick(uint32_t tick, UITimer* timer) = 0;

    protected:
        UIView* Target = nullptr;
        void DestroyAnimation(Animation* animation);
    };
}
