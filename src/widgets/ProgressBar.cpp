#include "ProgressBar.h"

namespace mc {
    ProgressBar::ProgressBar() {
        _createVisuals();
        _setupProperties();
    }

    Size ProgressBar::_measureSize() {
        return Size(fixedWidth, fixedHeight);
    }

    void ProgressBar::_onSetComputedSize(const Size& size) {
        float percentage =	static_cast<float>(value - minValue) /
                            static_cast<float>(maxValue - minValue);

        float progressWidth = size.width * percentage;

        d_progressVisual->customWidth = static_cast<uint32_t>(progressWidth);
    }

    void ProgressBar::_createVisuals() {
        d_bodyVisual = MakeRef<RectVisual>();
        backgroundColor.forwardAssignment(&d_bodyVisual->color);
        addCoreVisualElement(d_bodyVisual);

        d_progressVisual = MakeRef<RectVisual>();
        progressColor.forwardAssignment(&d_progressVisual->color);
        addCoreVisualElement(d_progressVisual);
    }

    void ProgressBar::_setupProperties() {
        fixedHeight = 20;
        fixedWidth = 220;
        minValue = 0;
        maxValue = 100;
        value = 33;
        backgroundColor = Color::gray;
        progressColor = Color::blue;
        backgroundColor.forwardEmittedEvents(this);
        progressColor.forwardEmittedEvents(this);
    }
} // namespace mc
