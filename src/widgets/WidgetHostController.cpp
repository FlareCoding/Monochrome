#include "WidgetHostController.h"
#include "BaseContainerWidget.h"
#include <core/InternalFlags.h>
#include <utils/Cursor.h>

namespace mc {
    WidgetHostController::WidgetHostController()
        : EventEmitter({"widgetTreeChanged"}) {}

    void WidgetHostController::addWidget(Shared<BaseWidget> widget) {
        CORE_ASSERT(!widget->getParent(), "Cannot add widget, widget already has a parent");
        CORE_ASSERT(
            !findWidget(widget->getID()),
            "Cannot add widget, widget with the given UUID already exists"
        );

        d_widgets.push_back(widget);
        _fireWidgetTreeChangedEvent();

        // Setup event listeners on top layer widgets
        widget->on("propertyChanged", [this](Shared<Event> e) {
            _fireWidgetTreeChangedEvent();
        });

        widget->on("requestedFocusGain", [this](Shared<Event> e) {
            BaseWidget* target = e->get<BaseWidget*>("target");

            _handlePotentialFocusChange(target);
        });

        widget->on("requestedFocusLoss", [this](Shared<Event> e) {
            _handlePotentialFocusChange(nullptr);
        });

        if (widget->isContainer()) {
            widget->on("childAdded", [this](Shared<Event> e) {
                _fireWidgetTreeChangedEvent();
            });

            widget->on("childRemoved", [this](Shared<Event> e) {
                _fireWidgetTreeChangedEvent();
            });
        }
    }

    bool WidgetHostController::removeWidget(Shared<BaseWidget> widget) {
        return removeWidget(widget->getID());
    }

    bool WidgetHostController::removeWidget(uuid_t uuid) {
        for (auto it = d_widgets.begin(); it != d_widgets.end(); ++it) {
            if (it->get()->getID() == uuid) {
                d_widgets.erase(it);

                it->get()->setParent(nullptr);
                _fireWidgetTreeChangedEvent();
                return true;
            }
        }

        return false;
    }

    void WidgetHostController::removeAllWidgets() {
        if (!d_widgets.size()) {
            return;
        }

        for (auto& widget : d_widgets) {
            widget->setParent(nullptr);
        }

        d_widgets.clear();
        _fireWidgetTreeChangedEvent();
    }

    Shared<BaseWidget> WidgetHostController::findWidget(uuid_t uuid) {
        for (auto& widget : d_widgets) {
            if (widget->getID() == uuid) {
                return widget;
            }
        }

        return nullptr;
    }

    void WidgetHostController::processMouseMovedEvent(Shared<MouseMovedEvent> event) {
        // Creating a thread-safe copy
        std::vector<Shared<BaseWidget>> widgetList(d_widgets);

        Position parentOffset(0, 0);
        _processMouseMovedEvent(widgetList, event, parentOffset);
    }

    void WidgetHostController::processMousePressedEvent(Shared<MouseButtonEvent> event) {
        // Creating a thread-safe copy
        std::vector<Shared<BaseWidget>> widgetList(d_widgets);

        Position parentOffset(0, 0);
        BaseWidget* newFocusedWidget = nullptr;

        _processMousePressedEvent(widgetList, event, parentOffset, newFocusedWidget);

        // Check if any widgets gained/lost focus
        _handlePotentialFocusChange(newFocusedWidget);
    }

    void WidgetHostController::processMouseReleasedEvent(Shared<MouseButtonEvent> event) {
        // Creating a thread-safe copy
        std::vector<Shared<BaseWidget>> widgetList(d_widgets);

        Position parentOffset(0, 0);
        _processMouseReleasedEvent(widgetList, event, parentOffset);
    }

    void WidgetHostController::processKeyDownEvent(Shared<KeyDownEvent> event) {
        // The key down event should only
        // be fired on the focused widget.
        if (d_focusedWidget) {
            d_focusedWidget->fireEvent("keyDown", event);
        }
    }

    void WidgetHostController::processKeyUpEvent(Shared<KeyUpEvent> event) {
    }

    void WidgetHostController::_fireWidgetTreeChangedEvent() {
        fireEvent("widgetTreeChanged", Event::empty);
    }

    void WidgetHostController::_handlePotentialFocusChange(BaseWidget* newFocusedWidget) {
        // Check if the currently focused widget has lost
        // focus and if a new widget has become focused.
        if (newFocusedWidget != d_focusedWidget) {
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

            if (newFocusedWidget) {
                // Set the internal focused flag to true
                // on the newly focused widget.
                setInternalFlag(
                    newFocusedWidget->getInternalFlags(),
                    InternalWidgetFlag::IsFocused,
                    true
                );

                newFocusedWidget->fireEvent("gainedFocus", Event::empty);
            }

            // Update the focused widget pointer
            d_focusedWidget = newFocusedWidget;
            _fireWidgetTreeChangedEvent();
        }
    }

    void WidgetHostController::_processMouseMovedEvent(
        std::vector<Shared<BaseWidget>>& widgets,
        Shared<MouseMovedEvent> event,
        Position& parentPositionOffset
    ) {
        for (auto& widget : widgets) {
            // Fire a default mouse moved event
            widget->fireEvent("mouseMoved", event);

            // Get the cursor position and location
            Position cursorLocation = event->getLocation();
            Distance movedDistance = event->getDistance();

            // Create and adjust the widget's layer frame
            Frame frame = widget->getLayerFrame();
            frame.position.x += parentPositionOffset.x;
            frame.position.y += parentPositionOffset.y;

            // Getting the widget's flags and determining if
            // the mouse was previously in the frame.
            uint64_t& widgetFlags = widget->getInternalFlags();
            bool mouseWasInFrame =
                getInternalFlag(widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame);

            // Checking if mouse is currently in the widget's frame
            bool isMouseInFrame = frame.containsPoint(cursorLocation);

            // If the two boolean flags are different, the mouse
            // was either hovered onto or off the widget's frame.
            if (isMouseInFrame && !mouseWasInFrame) {
                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::WidgetHoveredOn, true);

                widget->fireEvent("hoveredOn", Event::empty);

                // Set the widget-specific cursor type
                utils::Cursor::setActiveCursor(widget->cursorType);

                // Make sure to re-render the widget tree
                _fireWidgetTreeChangedEvent();
            } else if (!isMouseInFrame && mouseWasInFrame) {
                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::WidgetHoveredOn, false);

                setInternalFlag(
                    widgetFlags, InternalWidgetFlag::MouseDownOnWidget, false);

                widget->fireEvent("hoveredOff", Event::empty);

                // Reset the cursor icon
                utils::Cursor::setActiveCursor(DEFAULT_CURSOR_TYPE);

                // Make sure to re-render the widget tree
                _fireWidgetTreeChangedEvent();
            }

            // Updating internal flags
            setInternalFlag(
                widgetFlags, InternalWidgetFlag::IsMouseInWidgetFrame, isMouseInFrame);

            // Check if the view wants to stop propagating this event
            if (event->isHandled()) {
                return;
            }

            // Process the event for child widgets
            if (widget->isContainer()) {
                // Get the list of all children widgets and clone it
                std::vector<Shared<BaseWidget>> children(
                    std::static_pointer_cast<BaseContainerWidget>(widget)->getChildren()
                );

                // Adjust combined parent offset to keep child widget
                // frames positioned in relation to their parents.
                parentPositionOffset.x += widget->position->x;
                parentPositionOffset.y += widget->position->y;

                _processMouseMovedEvent(children, event, parentPositionOffset);

                // Restore the combined parent offset value
                parentPositionOffset.x -= widget->position->x;
                parentPositionOffset.y -= widget->position->y;
            }
        }
    }

    void WidgetHostController::_processMousePressedEvent(
        std::vector<Shared<BaseWidget>>& widgets,
        Shared<MouseButtonEvent> event,
        Position& parentPositionOffset,
        BaseWidget*& newFocusedWidget
    ) {
        for (auto& widget : widgets) {
            // Get the cursor position and location
            Position cursorLocation = event->getLocation();

            // Create and adjust the widget's layer frame
            Frame frame = widget->getLayerFrame();
            frame.position.x += parentPositionOffset.x;
            frame.position.y += parentPositionOffset.y;

            // Checking if mouse is currently in the widget's frame
            bool isMouseInFrame = frame.containsPoint(cursorLocation);

            if (!isMouseInFrame) {
                // Change the newly focused widget to null
                // only if the focused widget hasn't changed yet
                if (d_focusedWidget == newFocusedWidget) {
                    newFocusedWidget = nullptr;
                }

                continue;
            }

            // Updating internal flags to indicate that
            // the mouse is pressed on top of the widget.
            uint64_t& widgetFlags = widget->getInternalFlags();

            setInternalFlag(
                widgetFlags, InternalWidgetFlag::MouseDownOnWidget, true);

            // Set the temporary handle to the newly focused widget to current widget
            newFocusedWidget = widget.get();

            // Add the widget to the list of
            // widgets that mouse is pressed over.
            d_widgetsUnderMouseDown.push_back(widget);

            // Fire the mouseDown event
            widget->fireEvent("mouseDown", event);

            // Make sure to re-render the widget tree
            _fireWidgetTreeChangedEvent();

            // Check if the view wants to stop propagating this event
            if (event->isHandled()) {
                return;
            }

            // Process the event for child widgets
            if (widget->isContainer()) {
                // Get the list of all children widgets and clone it
                std::vector<Shared<BaseWidget>> children(
                    std::static_pointer_cast<BaseContainerWidget>(widget)->getChildren()
                );

                // Adjust combined parent offset to keep child widget
                // frames positioned in relation to their parents.
                parentPositionOffset.x += widget->position->x;
                parentPositionOffset.y += widget->position->y;

                _processMousePressedEvent(children, event, parentPositionOffset, newFocusedWidget);

                // Restore the combined parent offset value
                parentPositionOffset.x -= widget->position->x;
                parentPositionOffset.y -= widget->position->y;
            }

            // If a widget captures the event, don't
            // check for other widgets on the same z-level.
            return;
        }
    }

    void WidgetHostController::_processMouseReleasedEvent(
        std::vector<Shared<BaseWidget>>& widgets,
        Shared<MouseButtonEvent> event,
        Position& parentPositionOffset
    ) {
        for (auto& widget : widgets) {
            // Get the cursor position and location
            Position cursorLocation = event->getLocation();

            // Create and adjust the widget's layer frame
            Frame frame = widget->getLayerFrame();
            frame.position.x += parentPositionOffset.x;
            frame.position.y += parentPositionOffset.y;

            // Checking if mouse is currently in the widget's frame
            bool isMouseInFrame = frame.containsPoint(cursorLocation);

            if (!isMouseInFrame) {
                // If the widget had the mouse previously
                // pressed on it, remove it from the list.
                auto it = std::find(
                    d_widgetsUnderMouseDown.begin(),
                    d_widgetsUnderMouseDown.end(),
                    widget);

                if (it != d_widgetsUnderMouseDown.end()) {
                    // Fire the mouseUp event to indicate
                    // that the mouse is no longer pressed on it.
                    widget->fireEvent("mouseUp", event);
                    d_widgetsUnderMouseDown.erase(it);
                }

                continue;
            }

             // Updating internal flags to indicate that the mouse
            // is no longer pressed on the widget
            uint64_t& widgetFlags = widget->getInternalFlags();

            setInternalFlag(
                widgetFlags, InternalWidgetFlag::MouseDownOnWidget, false);

            // Fire the clicked and mouse up events
            widget->fireEvent("mouseUp", event);
            widget->fireEvent("clicked", event);

            // Make sure to re-render the widget tree
            _fireWidgetTreeChangedEvent();

            // Check if the view wants to stop propagating this event
            if (event->isHandled()) {
                return;
            }

            // Process the event for child widgets
            if (widget->isContainer()) {
                // Get the list of all children widgets and clone it
                std::vector<Shared<BaseWidget>> children(
                    std::static_pointer_cast<BaseContainerWidget>(widget)->getChildren()
                );

                // Adjust combined parent offset to keep child widget
                // frames positioned in relation to their parents.
                parentPositionOffset.x += widget->position->x;
                parentPositionOffset.y += widget->position->y;

                _processMouseReleasedEvent(children, event, parentPositionOffset);

                // Restore the combined parent offset value
                parentPositionOffset.x -= widget->position->x;
                parentPositionOffset.y -= widget->position->y;
            }
        }
    }
} // namespace mc
