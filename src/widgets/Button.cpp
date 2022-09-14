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
        // Check for a specific case when there is
        // no text, but instead a cover button.
        if (d_image && imagePlacement == Cover && label->text->empty()) {
            return d_image->getDesiredSize();
        }

        auto contentSize = label->getDesiredSize();

        if (d_image && imagePlacement == Icon) {
            auto imageSize = contentSize.height;
            contentSize.width += imageSize;
        }

        return contentSize;
    }

    void Button::_onSetComputedSize(const Size& size) {
        uint32_t iconOffset = 0;

        if (d_image) {
            if (imagePlacement == Icon) {
                d_image->position = Position(4, 0);
                d_image->setComputedSize(Size(
                    size.height - borderThickness,
                    size.height - borderThickness
                )); // Square

                iconOffset = size.height;

            } else if (imagePlacement == Cover) {
                d_image->position = Position(0, 0);
                d_image->setComputedSize(size); // Square
            }
        }

        label->setComputedSize(Size(size.width - iconOffset, size.height));
        label->position->x = iconOffset;

        Size secondaryLabelsSize = Size(
            size.width - d_secondaryTextPadding * 2 - iconOffset, size.height
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

        hoverOnColor = Color::transparent;
        hoverOnColor.forwardEmittedEvents(this);

        mousePressedColor = Color::transparent;
        mousePressedColor.forwardEmittedEvents(this);

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

        imagePlacement = Cover;
        imagePlacement.forwardEmittedEvents(this);

        on("hoveredOn", &Button::_onHoveredOn, this);
        on("hoveredOff", &Button::_onHoveredOff, this);
        on("mouseDown", &Button::_onMouseDown, this);
        on("mouseUp", &Button::_onMouseUp, this);
    }

    void Button::_onHoveredOn(Shared<Event> e) {
        if (hoverOnColor.get() != Color::transparent) {
            backgroundColor->r = hoverOnColor->r;
            backgroundColor->g = hoverOnColor->g;
            backgroundColor->b = hoverOnColor->b;
            backgroundColor->a = hoverOnColor->a;
            d_bodyVisual->color = hoverOnColor;
        } else {
            dimColor(backgroundColor);
            dimColor(d_bodyVisual->color);
        }

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

        if (mousePressedColor.get() != Color::transparent) {
            backgroundColor->r = mousePressedColor->r;
            backgroundColor->g = mousePressedColor->g;
            backgroundColor->b = mousePressedColor->b;
            backgroundColor->a = mousePressedColor->a;
            d_bodyVisual->color = mousePressedColor;
        } else {
            dimColor(backgroundColor);
            dimColor(d_bodyVisual->color);
        }

        e->stopPropagation();
        fireEvent("propertyChanged", Event::empty);
    }

    void Button::_onMouseUp(Shared<Event> e) {
        auto button = std::static_pointer_cast<MouseButtonEvent>(e)->getButton();
        if (button != MouseButton::Left) {
            return;
        }

        if (hoverOnColor.get() != Color::transparent) {
            backgroundColor->r = hoverOnColor->r;
            backgroundColor->g = hoverOnColor->g;
            backgroundColor->b = hoverOnColor->b;
            backgroundColor->a = hoverOnColor->a;
            d_bodyVisual->color = hoverOnColor;
        } else {
            undimColor(backgroundColor);
            undimColor(d_bodyVisual->color);
        }

        e->stopPropagation();
        fireEvent("propertyChanged", Event::empty);
    }

    void Button::setImage(Shared<Image> image) {
        if (d_image) {
            _removeChild(d_image->getID());
        }

        d_image = image;

        if (image) {
            _addChild(image);
        }
    }
} // namespace mc
