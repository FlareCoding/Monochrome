#pragma once
#include "VisualElement.h"

namespace mc {
class TextVisual : public VisualElement {
public:
    inline const VisualType type() const override { return VisualTypeText; }
    Size measureSize() const override;

    // Limit on the width of the text block
    PropertyObserver<uint32_t> widthLimit;

    // Text to be displayed on the label
    PropertyObserver<std::string> text;

    // Font of the text displayed
    PropertyObserver<std::string> font = PropertyObserver<std::string>("Arial");

    // Size of the text font
    PropertyObserver<uint32_t> fontSize = PropertyObserver<uint32_t>(12);

    // Possible options: "left", "right", "center"
    PropertyObserver<std::string> alignment = PropertyObserver<std::string>("center");

    // Possible options: "bold", "italic", "normal"
    PropertyObserver<std::string> fontStyle = PropertyObserver<std::string>("normal");

    // Possible options: "none", "word", "letter"
    PropertyObserver<std::string> wordWrapMode = PropertyObserver<std::string>("none");
};
} // namespace mc
