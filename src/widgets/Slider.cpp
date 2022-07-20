#include "Slider.h"
#include <events/MouseEvents.h>

namespace mc {
    Slider::Slider() {
        appendAllowedEvent("valueChanged");

        _setupProperties();
        _setupEventHandlers();
    }

    void Slider::_setupProperties() {
        cornerRadius = 1;
        cornerRadius.forwardEmittedEvents(this);

        minValue = 0;
        minValue.forwardEmittedEvents(this);

        maxValue = 100;
        maxValue.forwardEmittedEvents(this);

        value = 40;
        value.forwardEmittedEvents(this);

        increment = 10;
        increment.forwardEmittedEvents(this);

        showTickmarks = false;
        showTickmarks.forwardEmittedEvents(this);

        tickmarkColor = Color::lightGray;
        tickmarkColor.forwardEmittedEvents(this);

        circularKnob = true;
        circularKnob.forwardEmittedEvents(this);

        knobColor = Color::gray;
        knobColor.forwardEmittedEvents(this);

        completedValuesColor = Color(0, 200, 200);
        completedValuesColor.forwardEmittedEvents(this);

        color = Color::lightGray;
        size = { 140, 20 };
    }

    void Slider::_setupEventHandlers() {
        on("mouseDown", [this](Shared<Event> e) {
            d_mousePressed = true;
            d_positionInWindow = getPositionInWindow();

            auto clickedEvent = std::static_pointer_cast<MouseButtonEvent>(e);

            auto clickPosX = clickedEvent->getLocation().x;
            _onMouseEvent(clickPosX);
        });

        on("mouseUp", [this](Shared<Event> e) {
            d_mousePressed = false;
        });

        on("mouseMoved", [this](Shared<Event> e) {
            // Only react to mouse "dragging"
            if (!d_mousePressed) {
                return;
            }

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);
            int32_t mousePosX = mouseMovedEvent->getLocation().x;

            _onMouseEvent(mousePosX);
        });

        value.on("propertyChanged", [this](Shared<Event> e) {
            fireEvent("valueChanged", {
                { "value", value.get() }
            });
        });
    }

    void Slider::_onMouseEvent(int32_t mousePosX) {
        int32_t localMousePosX = mousePosX - d_positionInWindow.x;

        int32_t newSliderValue = value;

        // Checking the extreme values where the mouse
        // is outside of the bounds of the slider.
        if (localMousePosX < 0) {
            newSliderValue = minValue;
        } else if (localMousePosX > static_cast<int32_t>(size->width)) {
            newSliderValue = maxValue;
        } else {
            float percentage = static_cast<float>(localMousePosX) / static_cast<float>(size->width);
            int32_t percentageValue = static_cast<int32_t>(percentage * maxValue);

            newSliderValue = percentageValue + increment / 2;
            newSliderValue -= newSliderValue % increment;
        }

        if (value.get() != newSliderValue) {
            value = newSliderValue;
        }
    }
} // namespace mc
