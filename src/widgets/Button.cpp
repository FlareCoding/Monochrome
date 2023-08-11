#include "Button.h"
#include <events/MouseEvents.h>

namespace mc {
    Button::Button() {
        _createVisuals();
        _setupProperties();
    }

    Button::Button(const std::string& text) : Button() {
        label->text = text;
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

        auto secondaryLeftContentSize = d_secondaryLeftLabel->getDesiredSize();
        auto secondaryRightContentSize = d_secondaryRightLabel->getDesiredSize();

        contentSize.width += secondaryLeftContentSize.width;
        contentSize.width += secondaryRightContentSize.width;

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
        d_secondaryLeftLabel->visible = false;
        d_secondaryLeftLabel->text = "";
        d_secondaryLeftLabel->alignment = "left";
        d_secondaryLeftLabel->position->x = d_secondaryTextPadding;
        secondaryLeftText.on("propertyChanged", [this](Shared<Event> e) {
            if (d_secondaryLeftLabel->text->empty()) {
                d_secondaryLeftLabel->visible = false;
            } else {
                d_secondaryLeftLabel->visible = true;
            }
        });
        _addChildOffline(d_secondaryLeftLabel);

        d_secondaryRightLabel = MakeRef<Label>();
        d_secondaryRightLabel->visible = false;
        d_secondaryRightLabel->text = "";
        d_secondaryRightLabel->alignment = "right";
        d_secondaryRightLabel->position->x = d_secondaryTextPadding;
        secondaryRightText.on("propertyChanged", [this](Shared<Event> e) {
            if (d_secondaryRightLabel->text->empty()) {
                d_secondaryRightLabel->visible = false;
            } else {
                d_secondaryRightLabel->visible = true;
            }
        });
        _addChildOffline(d_secondaryRightLabel);

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
        label->horizontalPadding = 30;
        label->verticalPadding = 10;
        _addChildOffline(label);

        borderColor = Color::white;
        handleWidgetVisiblePropertyChange(borderColor);

        backgroundColor.forwardAssignment(&d_preservedBackgroundColor);
        backgroundColor = Color(72, 66, 84);
        handleWidgetVisiblePropertyChange(backgroundColor);

        cornerRadius = 6;
        handleWidgetVisiblePropertyChange(cornerRadius);

        borderThickness = 0;
        handleWidgetVisiblePropertyChange(borderThickness);

        enabled = true;
        handleWidgetVisiblePropertyChange(enabled);
        enabled.on("propertyChanged", &Button::_toggleEnabledState, this);

        hoverOnColor = Color::transparent;
        handleWidgetVisiblePropertyChange(hoverOnColor);

        mousePressedColor = Color::transparent;
        handleWidgetVisiblePropertyChange(mousePressedColor);

        secondaryLeftText = "";
        secondaryLeftText.forwardAssignment(&d_secondaryLeftLabel->text);
        handleWidgetVisiblePropertyChange(secondaryLeftText);

        secondaryRightText = "";
        secondaryRightText.forwardAssignment(&d_secondaryRightLabel->text);
        handleWidgetVisiblePropertyChange(secondaryRightText);

        cursorType.forwardAssignment(&label->cursorType);
        cursorType.forwardAssignment(&d_secondaryLeftLabel->cursorType);
        cursorType.forwardAssignment(&d_secondaryRightLabel->cursorType);
        cursorType = CursorType::Hand;

        imagePlacement = Icon;
        handleWidgetVisiblePropertyChange(imagePlacement);

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
            backgroundColor->dim();
            d_bodyVisual->color->dim();
        }

        requestRepaint();
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
            backgroundColor->dim();
            d_bodyVisual->color->dim();
        }

        e->stopPropagation();

        requestRepaint();
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
            backgroundColor->undim();
            d_bodyVisual->color->undim();
        }

        e->stopPropagation();

        requestRepaint();
    }

    void Button::_toggleEnabledState(Shared<Event> e) {
        if (d_isEnabled == enabled.get()) {
            return;
        }

        d_isEnabled = enabled.get();

        if (d_isEnabled) {
            enablePointerEvents();
            label->enablePointerEvents();

            backgroundColor->undim(3);
            d_bodyVisual->color->undim(3);

            auto labelColor = label->color;
            labelColor->undim(4);
            label->color = labelColor;
        } else {
            _onHoveredOff(e);
            disablePointerEvents();
            label->disablePointerEvents();

            backgroundColor->dim(3);
            d_bodyVisual->color->dim(3);

            auto labelColor = label->color;
            labelColor->dim(4);
            label->color = labelColor;
        }
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
