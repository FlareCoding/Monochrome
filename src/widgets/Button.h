#pragma once
#include "BaseWidget.h"

namespace mc
{
    class Button : public BaseWidget
    {
    public:
        Button();
        inline const std::string getType() const override { return "button"; }

        // Text to be displayed on the button
        PropertyObserver<std::string>   text;

        // Additional text that can optionally be displayed
        // on the far-right end of the button with the same
        // color as the main text. Empty by default.
        PropertyObserver<std::string>   secondaryRightText;

        // Additional text that can optionally be displayed
        // on the far-left end of the button with the same
        // color as the main text. Empty by default.
        PropertyObserver<std::string>   secondaryLeftText;

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

        // Color of the border around the button, transparent by default
        PropertyObserver<Color>         borderColor;

        // Width of the border around the button's body
        PropertyObserver<uint32_t>      borderSize;

        // Specifies whether or not to fill the button's body
        PropertyObserver<bool>          filled;

        // Specifies the width of the frame if 'filled' is false
        PropertyObserver<uint32_t>      stroke;

        // Corner radius of the button's frame
        PropertyObserver<uint32_t>      cornerRadius;

        // Color of the button when it's being hovered over with a mouse.
        // If the specified value is Color::transparent, the button's
        // body color will be decided automatically.
        PropertyObserver<Color>         onHoverColor;

        // Color of the button when the mouse is pressed down on it.
        // If the specified value is Color::transparent, the button's
        // body color will be decided automatically.
        PropertyObserver<Color>         onMousePressColor;

    private:
        void _setupProperties() override;
    };
}
