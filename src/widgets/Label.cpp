#include "Label.h"

namespace mc {
    Label::Label() {
        _createVisuals();
        _setupProperties();
    }

    Size Label::_measureSize() {
        auto textSize = d_visualElement->measureSize();

        return {
            horizontalPadding + textSize.width,
            verticalPadding + textSize.height
        };
    }

    void Label::_createVisuals() {
        // Text visual
        d_visualElement = MakeRef<TextVisual>();

        color.forwardAssignment(&d_visualElement->color);
        text.forwardAssignment(&d_visualElement->text);
        font.forwardAssignment(&d_visualElement->font);
        fontSize.forwardAssignment(&d_visualElement->fontSize);
        alignment.forwardAssignment(&d_visualElement->alignment);
        fontStyle.forwardAssignment(&d_visualElement->fontStyle);
        wordWrapMode.forwardAssignment(&d_visualElement->wordWrapMode);

        addCoreVisualElement(d_visualElement);
    }

    void Label::_setupProperties() {
        text = "Text";
        text.on("propertyChanged", [this](Shared<Event> e) {
            // Since the text will be taking up a potentially different 
            // amount of space, the layout needs to be recalculated.
            fireEvent("layoutChanged", Event::empty);
        });

        color = Color::white;
        color.forwardEmittedEvents(this);

        horizontalPadding = 5;
        horizontalPadding.forwardEmittedEvents(this);

        verticalPadding = 5;
        verticalPadding.forwardEmittedEvents(this);

        font.forwardEmittedEvents(this);
        fontSize.forwardEmittedEvents(this);
        fontStyle.forwardEmittedEvents(this);
        alignment.forwardEmittedEvents(this);
        wordWrapMode.forwardEmittedEvents(this);
    }
} // namespace mc
