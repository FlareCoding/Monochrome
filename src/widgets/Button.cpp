#include "Button.h"
#include "visuals/Visuals.h"

namespace mc {
    Button::Button() {
        _createVisuals();
        _setupProperties();
    }

    void Button::_createVisuals() {
        // Setup the button's body rectangle
        auto buttonBodyRect = MakeRef<RectVisual>();

        size.forwardAssignment(&buttonBodyRect->size);
        cornerRadius.forwardAssignment(&buttonBodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&buttonBodyRect->color);
        addCoreVisualElement(buttonBodyRect);

        // Setup the button's border
        auto buttonBorder = MakeRef<BorderVisual>();

        size.forwardAssignment(&buttonBorder->size);
        cornerRadius.forwardAssignment(&buttonBorder->cornerRadius);
        borderColor.forwardAssignment(&buttonBorder->color);
        borderThickness.forwardAssignment(&buttonBorder->thickness);
        showBorder.forwardAssignment(&buttonBorder->visible);
        addCoreVisualElement(buttonBorder);
    }

    void Button::_setupProperties() {
        d_label = MakeRef<Label>();
        _addChild(d_label);

        position.forwardAssignment(&d_label->position);
        size.forwardAssignment(&d_label->size);
        textColor.forwardAssignment(&d_label->color);
        text.forwardAssignment(&d_label->text);
        font.forwardAssignment(&d_label->font);
        fontSize.forwardAssignment(&d_label->fontSize);
        fontStyle.forwardAssignment(&d_label->fontStyle);
        alignment.forwardAssignment(&d_label->alignment);
        wordWrapMode.forwardAssignment(&d_label->wordWrapMode);

        size = { 140, 40 };
        backgroundColor = Color::gray;
        borderColor = Color::white;
        borderThickness = 2;
        cornerRadius = 2;
        showBorder = true;
        text = "Button";
    }
} // namespace mc
