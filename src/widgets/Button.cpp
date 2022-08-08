#include "Button.h"

namespace mc {
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
        label->forwardEmittedEvents(this);
        _addChild(label);

        borderColor = Color::white;
        borderColor.forwardEmittedEvents(this);

        backgroundColor = Color::gray;
        backgroundColor.forwardEmittedEvents(this);

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        borderThickness = 2;
        borderThickness.forwardEmittedEvents(this);
    }
} // namespace mc
