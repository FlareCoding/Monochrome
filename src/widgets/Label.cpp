#include "Label.h"
#include "visuals/Visuals.h"

namespace mc {
    Label::Label() {
        _createVisuals();
        _setupProperties();
    }

    void Label::_createVisuals() {
        // Text visual
        auto visual = MakeRef<TextVisual>();

        color.forwardAssignment(&visual->color);
        text.forwardAssignment(&visual->text);
        font.forwardAssignment(&visual->font);
        fontSize.forwardAssignment(&visual->fontSize);
        alignment.forwardAssignment(&visual->alignment);
        fontStyle.forwardAssignment(&visual->fontStyle);
        wordWrapMode.forwardAssignment(&visual->wordWrapMode);

        addCoreVisualElement(visual);
    }

    void Label::_setupProperties() {
        width = 140;
        height = 30;
        text = "Text";
        color = Color::white;
    }
} // namespace mc
