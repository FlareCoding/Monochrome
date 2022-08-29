#pragma once
#include "Core.h"

namespace mc {
enum InternalWidgetFlag : uint64_t {
    IsMouseInWidgetFrame = 1,
    WidgetHoveredOn,
    MouseDownOnWidget,
    IsFocused,
    IsMouseDraggable
};

enum InternalWindowFlag : uint64_t {
    WindowStyleBorderless = 1,
    WindowStyleClosable,
    WindowStyleMinimizable,
    WindowStyleMaximizable,
    WindowStyleTitled,
    WindowStyleResizable,
    WindowStyleDraggable
};

extern void setInternalFlag(uint64_t& flags, uint64_t flagToSet, bool enabled);
extern bool getInternalFlag(uint64_t flags, uint64_t flagToGet);
} // namespace mc
