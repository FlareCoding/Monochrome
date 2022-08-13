#pragma once
#include "Label.h"
#include "visuals/RectVisual.h"
#include "visuals/BorderVisual.h"
#include "visuals/TextVisual.h"

namespace mc {
class Entry : public BaseWidget {
public:
    Entry();

    PropertyObserver<uint32_t>  cornerRadius;
    PropertyObserver<uint32_t>  borderThickness;
    PropertyObserver<Color>     backgroundColor;
    PropertyObserver<Color>     borderColor;
    PropertyObserver<Color>     focusedBorderColor;

    // Text to be displayed in the entry
    PropertyObserver<std::string>   text;

    // Color of the text
    PropertyObserver<Color>         textColor;

    // Font of the text displayed
    PropertyObserver<std::string>   font;

    // Size of the text font
    PropertyObserver<uint32_t>      fontSize;

    // Possible options: "bold", "italic", "normal"
    PropertyObserver<std::string>   fontStyle;

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    const uint32_t d_textPadding = 5;

    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual> d_bodyVisual;
    Shared<BorderVisual> d_borderVisual;
    Shared<TextVisual> d_textVisual;

    void _gainedFocus(Shared<Event> e);
    void _lostFocus(Shared<Event> e);
};
} // namespace mc
