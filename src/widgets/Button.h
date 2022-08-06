#pragma once
#include "Label.h"

namespace mc {
class Button : public BaseWidget {
public:
    Button();

    // Radius of the button's corners
    PropertyObserver<uint32_t>      cornerRadius;

    // Thickness of the border around the button
    PropertyObserver<uint32_t>      borderThickness;

    // Color of the button's body
    PropertyObserver<Color>         backgroundColor;

    // Color of the border around the button
    PropertyObserver<Color>         borderColor;

    // Color of the text
    PropertyObserver<Color>         textColor;

    // Specifies whether there should be a border around the button
    PropertyObserver<bool>          showBorder;

    // Text to be displayed on the button
    PropertyObserver<std::string>   text;

    // Font of the text displayed
    PropertyObserver<std::string>   font;

    // Size of the text font
    PropertyObserver<uint32_t>      fontSize;

    // Possible options: "left", "right", "center"
    PropertyObserver<std::string>   alignment;

    // Possible options: "bold", "italic", "normal"
    PropertyObserver<std::string>   fontStyle;

    // Possible options: "none", "word", "letter"
    PropertyObserver<std::string>   wordWrapMode;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<Label> d_label;
};
} // namespace mc
