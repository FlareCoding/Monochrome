#include "Button.h"
#include <events/MouseEvents.h>

namespace mc {
    static void dimColor(PropertyObserver<Color>& color) {
        color->r -= 8;
        color->g -= 8;
        color->b -= 8;
    }

    static void undimColor(PropertyObserver<Color>& color) {
        color->r += 8;
        color->g += 8;
        color->b += 8;
    }

    Button::Button() {
        _createVisuals();
        _setupProperties();
    }

    Size Button::_measureSize() {
        return label->getDesiredSize();
    }

    void Button::_onSetComputedSize(const Size& size) {
        label->setComputedSize(size);
    }

    void Button::_createVisuals() {
        // Button's body rect
        d_bodyVisual = MakeRef<RectVisual>();
        cornerRadius.forwardAssignment(&d_bodyVisual->cornerRadius);
        backgroundColor.forwardAssignment(&d_bodyVisual->color);
        addCoreVisualElement(d_bodyVisual);

        // Button's border
        d_borderVisual = MakeRef<BorderVisual>();
        cornerRadius.forwardAssignment(&d_borderVisual->cornerRadius);
        borderColor.forwardAssignment(&d_borderVisual->color);
        borderThickness.forwardAssignment(&d_borderVisual->thickness);
        addCoreVisualElement(d_borderVisual);
    }

    void Button::_setupProperties() {
        label = MakeRef<Label>();
        label->text = "Button";
        label->color = Color(200, 200, 200);
        label->horizontalPadding = 30;
        label->verticalPadding = 10;
        cursorType.forwardAssignment(&label->cursorType);
        _addChild(label);

        borderColor = Color::white;
        borderColor.forwardEmittedEvents(this);

        backgroundColor.forwardEmittedEvents(this);
        backgroundColor.forwardAssignment(&d_preservedBackgroundColor);
        backgroundColor = Color::gray;

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        borderThickness = 2;
        borderThickness.forwardEmittedEvents(this);

        cursorType = CursorType::Hand;

        on("hoveredOn", &Button::_onHoveredOn, this);
        on("hoveredOff", &Button::_onHoveredOff, this);
        on("mouseDown", &Button::_onMouseDown, this);
        on("mouseUp", &Button::_onMouseUp, this);
    }

    void Button::_onHoveredOn(Shared<Event> e) {
        dimColor(backgroundColor);
        dimColor(d_bodyVisual->color);
        fireEvent("propertyChanged", Event::empty);
    }

    void Button::_onHoveredOff(Shared<Event> e) {
        backgroundColor = d_preservedBackgroundColor;
    }

    void Button::_onMouseDown(Shared<Event> e) {
        auto button = std::static_pointer_cast<MouseButtonEvent>(e)->getButton();
        if (button != MouseButton::Left) {
            return;
        }

        dimColor(backgroundColor);
        dimColor(d_bodyVisual->color);
        fireEvent("propertyChanged", Event::empty);
    }

    void Button::_onMouseUp(Shared<Event> e) {
        auto button = std::static_pointer_cast<MouseButtonEvent>(e)->getButton();
        if (button != MouseButton::Left) {
            return;
        }

        undimColor(backgroundColor);
        undimColor(d_bodyVisual->color);
        fireEvent("propertyChanged", Event::empty);
    }
} // namespace mc
