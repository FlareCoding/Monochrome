#include "Panel.h"

namespace mc {
    Panel::Panel() {
        _setupProperties();
    }

    void Panel::_setupProperties() {
        filled = true;
        filled.forwardEmittedEvents(this);

        stroke = 0;
        stroke.forwardEmittedEvents(this);

        cornerRadius = 4;
        cornerRadius.forwardEmittedEvents(this);
    }
} // namespace mc
