#include "Animation.h"
#include "Animations.h"

#include <vector>
#include <mutex>

namespace mc
{
    std::vector<Ref<Animation>> s_RunningAnimations;
    std::mutex s_AnimationManagementMutex;

    void Animation::DestroyAnimation(Animation* animation)
    {
        s_AnimationManagementMutex.lock();

        for (auto it = s_RunningAnimations.begin(); it != s_RunningAnimations.end(); it++)
        {
            if (it->get() == animation)
            {
                s_RunningAnimations.erase(it);
                break;
            }
        }

        s_AnimationManagementMutex.unlock();
    }

    Ref<Animation> Animation::Create(AnimationType type, UIView* target)
    {
        Ref<Animation> instance = nullptr;
        switch (type)
        {
        case AnimationType::FadeOut:
        {
            instance = MakeRef<FadeOutAnimation>();
            break;
        }
        case AnimationType::FadeIn:
        {
            instance = MakeRef<FadeInAnimation>();
            break;
        }
        case AnimationType::Translation:
        {
            instance = MakeRef<TranslationAnimation>();
            break;
        }
        case AnimationType::Scale:
        {
            instance = MakeRef<ScaleAnimation>();
            break;
        }
        case AnimationType::Custom:
        {
            instance = MakeRef<CustomAnimation>();
            break;
        }
        default: break;
        }

        if (instance)
        {
            instance->Target = target;
            s_RunningAnimations.push_back(instance);
        }

        return instance;
    }
}
