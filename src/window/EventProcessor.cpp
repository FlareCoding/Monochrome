#include "EventProcessor.h"
#include <core/InternalFlags.h>

namespace mc {
    void EventProcessor::processMouseDownEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        d_rootWidget->fireEvent("mouseDown", Event::empty);

        Position positionOffset = Position(0, 0);

        // Pointer to the widget that the focus
        // should potentially switch to unless it
        // is the same widget being pressed on again.
        Shared<BaseWidget> focusChangeCandidate = nullptr;

        _processMouseDownEvent(
            std::static_pointer_cast<MouseButtonEvent>(event),
            d_rootWidget->_getChildren(),
            positionOffset,
            focusChangeCandidate
        );

        _handlePotentialFocusChanged(focusChangeCandidate);
    }

    void EventProcessor::processMouseUpEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        d_rootWidget->fireEvent("mouseUp", Event::empty);

        Position positionOffset = Position(0, 0);

        _processMouseUpEvent(
            std::static_pointer_cast<MouseButtonEvent>(event),
            d_rootWidget->_getChildren(),
            positionOffset
        );
    }

    void EventProcessor::processMouseMovedEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        d_rootWidget->fireEvent("mouseMoved", Event::empty);

        Position positionOffset = Position(0, 0);

        _processMouseMovedEvent(
            std::static_pointer_cast<MouseMovedEvent>(event),
            d_rootWidget->_getChildren(),
            positionOffset
        );
    }

    void EventProcessor::processKeyDownEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }
    }

    void EventProcessor::processKeyUpEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }
    }

    void EventProcessor::_handlePotentialFocusChanged(Shared<BaseWidget>& candidate) {
        // Check if the currently focused widget has lost
        // focus and if a new widget has become focused.
        if (candidate != d_focusedWidget) {
            if (d_focusedWidget) {
                // Set the internal focused flag to false
                // on the previously focused widget.
                setInternalFlag(
                    d_focusedWidget->getInternalFlags(),
                    InternalWidgetFlag::IsFocused,
                    false
                );

                d_focusedWidget->fireEvent("lostFocus", Event::empty);
            }

            if (candidate) {
                // Set the internal focused flag to true
                // on the newly focused widget.
                setInternalFlag(
                    candidate->getInternalFlags(),
                    InternalWidgetFlag::IsFocused,
                    true
                );

                candidate->fireEvent("gainedFocus", Event::empty);
            }

            // Update the focused widget pointer
            d_focusedWidget = candidate;
        }
    }

    void EventProcessor::_processMouseDownEvent(
        Shared<MouseButtonEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset,
        Shared<BaseWidget>& focusChangeCandidate
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Calculate the widget's runtime absolute position and size
            auto widgetPosition = widget->position + positionOffset;
            auto widgetSize = widget->getComputedSize();

            // Get the bounding box around the widget
            auto widgetFrame = Frame(widgetPosition, widgetSize);

            bool isMouseInFrame = widgetFrame.containsPoint(event->getLocation());

            if (isMouseInFrame) {
                // Set the internal widget flags
                setInternalFlag(widgetFlags, InternalWidgetFlag::MouseDownOnWidget, true);

                // Set the temporary handle to the newly focused widget to current widget
                focusChangeCandidate = widget;

                // Fire the event
                widget->fireEvent("mouseDown", event);

                // If the event is handled, return
                if (event->isHandled()) {
                    return;
                }

                // Process the event for all the children
                _processMouseDownEvent(
                    event,
                    widget->_getChildren(),
                    widgetPosition,
                    focusChangeCandidate
                );

                // Since the widget received mouseDown event on the current
                // layer, it means it was on top of all other widgets at
                // that position and the event should stop getting processed.
                break;
            }
        }
    }

    void EventProcessor::_processMouseUpEvent(
        Shared<MouseButtonEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Calculate the widget's runtime absolute position and size
            auto widgetPosition = widget->position + positionOffset;
            auto widgetSize = widget->getComputedSize();

            // Get the bounding box around the widget
            auto widgetFrame = Frame(widgetPosition, widgetSize);

            bool isMouseInFrame = widgetFrame.containsPoint(event->getLocation());

            if (isMouseInFrame) {
                widget->fireEvent("mouseUp", event);
                widget->fireEvent("clicked", event);

                // Set the internal widget flags
                setInternalFlag(widgetFlags, InternalWidgetFlag::MouseDownOnWidget, false);

                // If the event is handled, return
                if (event->isHandled()) {
                    return;
                }

                // Process the event for all the children
                _processMouseUpEvent(event, widget->_getChildren(), widgetPosition);

                // Since the widget received mouseDown event on the current
                // layer, it means it was on top of all other widgets at
                // that position and the event should stop getting processed.
                break;
            }
        }
    }

    void EventProcessor::_processMouseMovedEvent(
        Shared<MouseMovedEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Determine if the mouse was previously in the widget's frame
            bool wasMouseInFrame =
                    getInternalFlag(widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame);

            // Calculate the widget's runtime absolute position and size
            auto widgetPosition = widget->position + positionOffset;
            auto widgetSize = widget->getComputedSize();

            // Get the bounding box around the widget
            auto widgetFrame = Frame(widgetPosition, widgetSize);

            bool isMouseInFrame = widgetFrame.containsPoint(event->getLocation());

            // Mouse entered the frame for the first time
            if (isMouseInFrame && !wasMouseInFrame) {
                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame, true);

                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::WidgetHoveredOn, true);

                widget->fireEvent("hoveredOn", Event::empty);

                // Set the widget-specific cursor type
                utils::Cursor::setActiveCursor(widget->cursorType);
            }

            // Mouse just left the frame
            if (!isMouseInFrame && wasMouseInFrame) {
                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame, false);

                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::WidgetHoveredOn, false);

                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::MouseDownOnWidget, false);

                widget->fireEvent("hoveredOff", Event::empty);

                // Reset the cursor icon
                utils::Cursor::setActiveCursor(DEFAULT_CURSOR_TYPE);
            }

            // If the event is handled, return
            if (event->isHandled()) {
                return;
            }

            // Process the event for all the children
            _processMouseMovedEvent(event, widget->_getChildren(), widgetPosition);
        }
    }
} // namespace mc
