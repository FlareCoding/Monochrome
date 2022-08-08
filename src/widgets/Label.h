#pragma once
#include "BaseWidget.h"
#include "visuals/TextVisual.h"

namespace mc {
class Label : public BaseWidget {
public:
    Label();

    // Horizontal adding between the frame of the label and the text
    PropertyObserver<uint32_t>      horizontalPadding;

    // Vertical adding between the frame of the label and the text
    PropertyObserver<uint32_t>      verticalPadding;

    // Color of the text
    PropertyObserver<Color>         color;

    // Text to be displayed on the label
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

protected:
    Size _measureSize() override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<TextVisual> d_visualElement;
};
} // namespace mc
