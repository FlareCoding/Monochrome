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
        font = "Arial";
        fontSize = 12;
        fontStyle = "normal";
        alignment = "center";
        wordWrapMode = "none";
        text = "Text";

        handleWidgetLayoutPropertyChange(text);
        handleWidgetLayoutPropertyChange(font);
        handleWidgetLayoutPropertyChange(fontSize);
        handleWidgetLayoutPropertyChange(fontStyle);
        handleWidgetLayoutPropertyChange(wordWrapMode);

        color = Color::white;
        handleWidgetVisiblePropertyChange(color);

        horizontalPadding = 5;
        handleWidgetVisiblePropertyChange(horizontalPadding);

        verticalPadding = 5;
        handleWidgetVisiblePropertyChange(verticalPadding);

        handleWidgetVisiblePropertyChange(font);
        handleWidgetVisiblePropertyChange(fontSize);
        handleWidgetVisiblePropertyChange(fontStyle);
        handleWidgetVisiblePropertyChange(alignment);
        handleWidgetVisiblePropertyChange(wordWrapMode);
    }
} // namespace mc
