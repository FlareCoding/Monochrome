#pragma once
#include "VisualElement.h"

namespace mc {
class TextVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeText; }

    std::string     text;
    std::string     font;
    uint32_t        fontSize;

    // Text to be displayed on the label
    std::string text;

    // Font of the text displayed
    std::string font;

    // Size of the text font
    uint32_t fontSize;

    // Possible options: "left", "right", "center"
    std::string alignment;

    // Possible options: "bold", "italic", "normal"
    std::string fontStyle;

    // Possible options: "none", "word", "letter"
    std::string wordWrapMode;
};
} // namespace mc
