#pragma once
#include "BaseWidget.h"
#include <events/EventEmitter.h>
#include <events/MouseEvents.h>
#include <events/KeyboardEvents.h>

namespace mc
{
    class WidgetHostController : public EventEmitter
    {
    public:
        WidgetHostController();

        void addWidget(Shared<BaseWidget> widget);
        bool removeWidget(Shared<BaseWidget> widget);
        bool removeWidget(uuid_t uuid);
        void removeAllWidgets();
        Shared<BaseWidget> findWidget(uuid_t uuid);

        inline std::vector<Shared<BaseWidget>>& getSceneWidgetList() { return d_widgets; }

        void processMouseMovedEvent(Shared<MouseMovedEvent> event);
        void processMousePressedEvent(Shared<MouseButtonEvent> event);
        void processMouseReleasedEvent(Shared<MouseButtonEvent> event);
        void processKeyDownEvent(Shared<KeyDownEvent> event);
        void processKeyUpEvent(Shared<KeyUpEvent> event);

    private:
        std::vector<Shared<BaseWidget>> d_widgets;
        BaseWidget* d_focusedWidget = nullptr;
        std::vector<Shared<BaseWidget>> d_widgetsUnderMouseDown;

        void _fireWidgetTreeChangedEvent();

        void _handlePotentialFocusChange(BaseWidget* newFocusedWidget);

        void _processMouseMovedEvent(
            std::vector<Shared<BaseWidget>>& widgets,
            Shared<MouseMovedEvent> event,
            Position& parentPositionOffset
        );

        void _processMousePressedEvent(
            std::vector<Shared<BaseWidget>>& widgets,
            Shared<MouseButtonEvent> event,
            Position& parentPositionOffset,
            BaseWidget*& newFocusedWidget
        );

        void _processMouseReleasedEvent(
            std::vector<Shared<BaseWidget>>& widgets,
            Shared<MouseButtonEvent> event,
            Position& parentPositionOffset
        );
    };
}
