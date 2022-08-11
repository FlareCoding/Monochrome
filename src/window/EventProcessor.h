#pragma once
#include <events/MouseEvents.h>
#include <events/KeyboardEvents.h>
#include <widgets/BaseWidget.h>

namespace mc {
class EventProcessor {
public:
    EventProcessor() = default;

    void processMouseDownEvent(Shared<Event> event);
    void processMouseUpEvent(Shared<Event> event);
    void processMouseMovedEvent(Shared<Event> event);
    void processKeyDownEvent(Shared<Event> event);
    void processKeyUpEvent(Shared<Event> event);

    inline void setRootWidget(Shared<BaseWidget> root) { d_rootWidget = root; }

private:
    Shared<BaseWidget> d_rootWidget = nullptr;
    Shared<BaseWidget> d_focusedWidget = nullptr;

    void _handlePotentialFocusChanged(Shared<BaseWidget>& candidate);

    void _processMouseDownEvent(
        Shared<MouseButtonEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset,
        Shared<BaseWidget>& focusChangeCandidate
    );

    void _processMouseUpEvent(
        Shared<MouseButtonEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    );

    void _processMouseMovedEvent(
        Shared<MouseMovedEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    );
};
} // namespace mc
