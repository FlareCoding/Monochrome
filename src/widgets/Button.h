#pragma once
#include "Label.h"
#include "visuals/RectVisual.h"
#include "visuals/BorderVisual.h"

namespace mc {
class Button : public BaseWidget {
public:
    Button();

    PropertyObserver<uint32_t>  cornerRadius;
    PropertyObserver<uint32_t>  borderThickness;
    PropertyObserver<Color>     backgroundColor;
    PropertyObserver<Color>     borderColor;

    // Optional text that can be displayed on the left side of the button
    PropertyObserver<std::string> secondaryLeftText;

    // Optional text that can be displayed on the right side of the button
    PropertyObserver<std::string> secondaryRightText;

    // Custom color to replace the background color when the button is hovered on
    PropertyObserver<Color>       hoverOnColor;

    // Custom color to replace the background color when the button is pressed on
    PropertyObserver<Color>       mousePressedColor;

    Shared<Label> label;

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual> d_bodyVisual;
    Shared<BorderVisual> d_borderVisual;

    Shared<Label> d_secondaryLeftLabel;
    Shared<Label> d_secondaryRightLabel;

    const uint32_t d_secondaryTextPadding = 5;

    PropertyObserver<Color> d_preservedBackgroundColor;

    void _onHoveredOn(Shared<Event> e);
    void _onHoveredOff(Shared<Event> e);
    void _onMouseDown(Shared<Event> e);
    void _onMouseUp(Shared<Event> e);
};
} // namespace mc
