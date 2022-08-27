#pragma once
#include "Event.h"
#include <widgets/Layer.h>

namespace mc {
enum class MouseButton {
    Left = 1,
    Right,
    Middle
};

class MouseMovedEvent : public Event {
public:
    inline Position getLocation() { return get<Position>("location");  }
    inline Position getScreenLocation() { return get<Position>("screenLocation"); }
    inline Distance getDistance() { return get<Distance>("distance");  }
};

class MouseButtonEvent : public Event {
public:
    inline Position getLocation() { return get<Position>("location");  }
    inline Position getScreenLocation() { return get<Position>("screenLocation");  }
    inline MouseButton getButton() { return get<MouseButton>("button"); }
};
} // namespace mc
