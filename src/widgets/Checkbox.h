#pragma once
#include "BaseWidget.h"

namespace mc {
class Checkbox : public BaseWidget {
public:
    Checkbox();
    inline const std::string getType() const override { return "checkbox"; }

    // Specifies the checked state of the checkbox
    PropertyObserver<bool>          checked;

    // Specifies whether or not the checkmark should
    // be displayed when the checkbox is checked.
    PropertyObserver<bool>          displayCheckmark;

    // Space between the box that can be checked and the text
    PropertyObserver<uint32_t>      textMargin;

    // Text to be displayed next to the checkbox
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

    // Color of the border around the checkbox, transparent by default
    PropertyObserver<Color>         borderColor;

    // Color of the checkbox when it is checked
    PropertyObserver<Color>         checkedColor;

    // Color of the checkmark inside the box when checked
    PropertyObserver<Color>         checkmarkColor;

    // Width of the border around the checkbox's body
    PropertyObserver<uint32_t>      borderSize;

    // Specifies whether or not to fill the checkbox's body
    PropertyObserver<bool>          filled;

    // Specifies the width of the frame if 'filled' is false
    PropertyObserver<uint32_t>      stroke;

    // Corner radius of the checkbox's frame
    PropertyObserver<uint32_t>      cornerRadius;

private:
    void _setupProperties() override;
};
} // namespace mc
