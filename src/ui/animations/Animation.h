#pragma once
#include <ui/UITimer.h>
#include <ui/UIView.h>

namespace mc
{
    enum class AnimationType
    {
        FadeOut,
        FadeIn,
        Translation,
        Scale
    };

#define ANIMATION_INSTANCE_CONSTRUCTOR \
        m_Timer.SetOnTickFunction([this](uint32_t tick, UITimer* timer) { OnTick(tick, timer); });

#define AnimCast(type, anim) std::dynamic_pointer_cast<type>(anim)

    class Animation
    {
    public:
        static Ref<Animation> Create(AnimationType type, UIView* target);
        virtual ~Animation() = default;

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
