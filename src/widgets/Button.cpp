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

        Size secondaryLabelsSize = Size(
            size.width - d_secondaryTextPadding * 2, size.height
        );

        d_secondaryLeftLabel->setComputedSize(secondaryLabelsSize);
        d_secondaryRightLabel->setComputedSize(secondaryLabelsSize);
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
        d_secondaryLeftLabel = MakeRef<Label>();
        d_secondaryLeftLabel->text = "";
        d_secondaryLeftLabel->alignment = "left";
        d_secondaryLeftLabel->position->x = d_secondaryTextPadding;
        _addChild(d_secondaryLeftLabel);

        d_secondaryRightLabel = MakeRef<Label>();
        d_secondaryRightLabel->text = "";
        d_secondaryRightLabel->alignment = "right";
        d_secondaryRightLabel->position->x = d_secondaryTextPadding;
        _addChild(d_secondaryRightLabel);

        label = MakeRef<Label>();
        label->color.forwardAssignment(&d_secondaryLeftLabel->color);
        label->color.forwardAssignment(&d_secondaryRightLabel->color);
        label->font.forwardAssignment(&d_secondaryLeftLabel->font);
        label->font.forwardAssignment(&d_secondaryRightLabel->font);
        label->fontSize.forwardAssignment(&d_secondaryLeftLabel->fontSize);
        label->fontSize.forwardAssignment(&d_secondaryRightLabel->fontSize);
        label->fontStyle.forwardAssignment(&d_secondaryLeftLabel->fontStyle);
        label->fontStyle.forwardAssignment(&d_secondaryRightLabel->fontStyle);
        label->text = "Button";
        label->color = Color(200, 200, 200);
        label->horizontalPadding = 30;
        label->verticalPadding = 10;
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

        secondaryLeftText = "";
        secondaryLeftText.forwardAssignment(&d_secondaryLeftLabel->text);
        secondaryLeftText.forwardEmittedEvents(this);

        secondaryRightText = "";
        secondaryRightText.forwardAssignment(&d_secondaryRightLabel->text);
        secondaryRightText.forwardEmittedEvents(this);

        cursorType.forwardAssignment(&label->cursorType);
        cursorType.forwardAssignment(&d_secondaryLeftLabel->cursorType);
        cursorType.forwardAssignment(&d_secondaryRightLabel->cursorType);
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
