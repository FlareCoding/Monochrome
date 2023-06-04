#include "EventProcessor.h"
#include <core/InternalFlags.h>

namespace mc {
    using WidgetList_t = std::vector<Shared<BaseWidget>>;
    static WidgetList_t cloneWidgetList(WidgetList_t& original) {
        WidgetList_t clone(original);
        return clone;
    }

    EventProcessor::EventProcessor() {
        appendAllowedEvent("widgetTreeChanged");
    }

    void EventProcessor::processMouseDownEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        d_rootWidget->fireEvent("mouseDown", event, d_rootWidget.get());

        Position positionOffset = Position(0, 0);

        // Pointer to the widget that the focus
        // should potentially switch to unless it
        // is the same widget being pressed on again.
        Shared<BaseWidget> focusChangeCandidate = d_rootWidget;

        _processMouseDownEvent(
            std::static_pointer_cast<MouseButtonEvent>(event),
            cloneWidgetList(d_rootWidget->_getChildren()),
            positionOffset,
            focusChangeCandidate
        );

        handlePotentialFocusChanged(focusChangeCandidate);
    }

    void EventProcessor::processMouseUpEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        d_rootWidget->fireEvent("mouseUp", event, d_rootWidget.get());

        Position positionOffset = Position(0, 0);

        _processMouseUpEvent(
            std::static_pointer_cast<MouseButtonEvent>(event),
            cloneWidgetList(d_rootWidget->_getChildren()),
            positionOffset
        );
    }

    void EventProcessor::processMouseMovedEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        auto mme = std::static_pointer_cast<MouseMovedEvent>(event);

        d_rootWidget->fireEvent("mouseMoved", {
            { "location", mme->getLocation() }
        }, d_rootWidget.get());

        Position positionOffset = Position(0, 0);

        _processMouseMovedEvent(
            mme,
            d_rootWidget->_getChildren(),
            positionOffset
        );
    }
    
    void EventProcessor::processMouseScrolledEvent(Shared<Event> event) {
        if (!d_rootWidget) {
            return;
        }

        auto mse = std::static_pointer_cast<MouseScrolledEvent>(event);

        d_rootWidget->fireEvent("mouseScrolled", event, d_rootWidget.get());

        Position positionOffset = Position(0, 0);

        _processMouseScrolledEvent(
            mse,
            d_rootWidget->_getChildren(),
            positionOffset
        );
    }
    
    void EventProcessor::processKeyDownEvent(Shared<Event> event) {
        if (!d_rootWidget || !d_focusedWidget) {
            return;
        }

        d_focusedWidget->fireEvent("keyDown", event);
    }

    void EventProcessor::processKeyUpEvent(Shared<Event> event) {
        if (!d_rootWidget || !d_focusedWidget) {
            return;
        }

        d_focusedWidget->fireEvent("keyUp", event);
    }

    void EventProcessor::handlePotentialFocusChanged(Shared<BaseWidget>& candidate) {
        handlePotentialFocusChanged(candidate.get());
    }

    void EventProcessor::handlePotentialFocusChanged(BaseWidget* candidate) {
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

                d_focusedWidget->fireEvent("lostFocus", Event::empty, d_focusedWidget);
            }

            if (candidate) {
                // Set the internal focused flag to true
                // on the newly focused widget.
                setInternalFlag(
                    candidate->getInternalFlags(),
                    InternalWidgetFlag::IsFocused,
                    true
                );

                candidate->fireEvent("gainedFocus", Event::empty, candidate);
            }

            // Update the focused widget pointer
            d_focusedWidget = candidate;
        }
    }

    void EventProcessor::_processMouseDownEvent(
        Shared<MouseButtonEvent> event,
        const std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset,
        Shared<BaseWidget>& focusChangeCandidate
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Ignore invisible widgets
            if (!widget->visible) {
                continue;
            }

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
                widget->fireEvent("mouseDown", event, widget.get());

                // If the event is handled, return
                if (event->isHandled()) {
                    return;
                }

                // Process the event for all the children
                _processMouseDownEvent(
                    event,
                    cloneWidgetList(widget->_getChildren()),
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
        const std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Ignore invisible widgets
            if (!widget->visible) {
                continue;
            }

            // Calculate the widget's runtime absolute position and size
            auto widgetPosition = widget->position + positionOffset;
            auto widgetSize = widget->getComputedSize();

            // Get the bounding box around the widget
            auto widgetFrame = Frame(widgetPosition, widgetSize);

            bool isMouseInFrame = widgetFrame.containsPoint(event->getLocation());

            // This logic is used for widgets that are draggable such
            // as slider knob or scrollbars that need to process mouse
            // release event potentially when the mouse is off the widget.
            bool wasMousePressed =
                getInternalFlag(widgetFlags, InternalWidgetFlag::MouseDownOnWidget) &&
                getInternalFlag(widgetFlags, InternalWidgetFlag::IsMouseDraggable);

            if (isMouseInFrame || wasMousePressed) {
                widget->fireEvent("mouseUp", event, widget.get());
                widget->fireEvent("clicked", event, widget.get());

                // Set the internal widget flags
                setInternalFlag(widgetFlags, InternalWidgetFlag::MouseDownOnWidget, false);

                // If the event is handled, return
                if (event->isHandled()) {
                    return;
                }

                // Process the event for all the children
                _processMouseUpEvent(
                    event,
                    cloneWidgetList(widget->_getChildren()),
                    widgetPosition
                );

                // Since the widget received mouseUp event on the current
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

            // Ignore invisible widgets
            if (!widget->visible) {
                continue;
            }

            // Immediately fire the mouse moved event
            widget->fireEvent("mouseMoved", event, widget.get());

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

                widget->fireEvent("hoveredOn", event, widget.get());

                // Set the widget-specific cursor type
                utils::Cursor::setActiveCursor(widget->cursorType);

                // Tell the window to redraw
                fireEvent("widgetTreeChanged", Event::empty);
            }

            // Mouse just left the frame
            if (!isMouseInFrame && wasMouseInFrame) {
                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame, false);

                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::WidgetHoveredOn, false);

                widget->fireEvent("hoveredOff", event, widget.get());

                // Reset the cursor icon
                utils::Cursor::setActiveCursor(DEFAULT_CURSOR_TYPE);

                // Tell the window to redraw
                fireEvent("widgetTreeChanged", Event::empty);
            }

            // If the event is handled, return
            if (event->isHandled()) {
                return;
            }

            // Process the event for all the children
            _processMouseMovedEvent(event, widget->_getChildren(), widgetPosition);
        }
    }
    
    void EventProcessor::_processMouseScrolledEvent(
        Shared<MouseScrolledEvent> event,
        std::vector<Shared<BaseWidget>>& widgets,
        Position& positionOffset
    ) {
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            auto& widget = *it;
            auto& widgetFlags = widget->getInternalFlags();

            // Ignore invisible widgets
            if (!widget->visible) {
                continue;
            }

            // Calculate the widget's runtime absolute position and size
            auto widgetPosition = widget->position + positionOffset;
            auto widgetSize = widget->getComputedSize();

            // First process the event for all children (bottom-up)
            _processMouseScrolledEvent(event, widget->_getChildren(), widgetPosition);

            // Get the bounding box around the widget
            auto widgetFrame = Frame(widgetPosition, widgetSize);

            bool isMouseInFrame = widgetFrame.containsPoint(event->getLocation());

            if (isMouseInFrame) {
                widget->fireEvent("mouseScrolled", event, widget.get());
            }
        }
    }
} // namespace mc
