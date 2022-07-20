#pragma once
#include <core/Core.h>
#include <widgets/Layer.h>
#include <vector>
#include <map>

#define MAIN_SCREEN_CONTAINER_NAME "mcScreenContainer"

namespace mc::utils {
class VirtualContainer {
public:
    Frame containerFrame;
    std::map<uint64_t, Frame> children;

    Position insertChild(
        uint64_t id,
        const Size& childSize,
        const Position& anchorOrigin,
        const Size& anchorElementSize,
        OverflowDirection overflow
    );

    void removeChild(uint64_t id);

private:
    bool framesColliding(
        Frame& first,
        Frame& second,
        OverflowDirection preferredOverflow,
        Distance& outResolutionDistance
    );
};

class PlacementConstraintSystem {
public:
    static void registerContainer(const std::string& name, const Size& size);
    static bool hasContainer(const std::string& name);
    static VirtualContainer* getContainer(const std::string& name);
    
    static void updateContainer(
        const std::string& name,
        const Position& position,
        const Size& size
    );

private:
    static std::map<std::string, VirtualContainer> s_containers;
};
} // namespace mc::utils
