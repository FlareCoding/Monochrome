#pragma once
#include "BaseWidget.h"

namespace mc {
class Slider : public BaseWidget {
public:
    Slider();
    inline const std::string getType() const override { return "slider"; }

    // Corner radius of the slider's body
    PropertyObserver<uint32_t>      cornerRadius;

    // Minimum possible value the slider can go to
    PropertyObserver<int32_t>       minValue;

    // Maximum possible value the slider can go to
    PropertyObserver<int32_t>       maxValue;

    // Current value of the slider
    PropertyObserver<int32_t>       value;

    // Increment by how much the value can increase at a time
    PropertyObserver<int32_t>       increment;

    // Indicates whether tickmarks should be displayed
    PropertyObserver<bool>          showTickmarks;

    // The color of tickmarks
    PropertyObserver<Color>         tickmarkColor;

    // Specifies whether to display the slider knob as a circle or a rectangle (default)
    PropertyObserver<bool>          circularKnob;

    // The color of the slider knob
    PropertyObserver<Color>         knobColor;

    // When the slider knob is moved, the region
    // behind the knob will be display this color.
    PropertyObserver<Color>         completedValuesColor;

private:
    void _setupProperties() override;
    void _setupEventHandlers();

    bool d_mousePressed = false;
    Position d_positionInWindow;

    void _onMouseEvent(int32_t mousePosX);
};
} // namespace mc
