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
        _computeProgressVisualSize();
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
        minValue.forwardEmittedEvents(this);
        minValue.on("propertyChanged", [this](Shared<Event> e) {
            _computeProgressVisualSize();
        });

        maxValue = 100;
        maxValue.forwardEmittedEvents(this);
        maxValue.on("propertyChanged", [this](Shared<Event> e) {
            _computeProgressVisualSize();
        });

        value = 33;
        value.forwardEmittedEvents(this);
        value.on("propertyChanged", [this](Shared<Event> e) {
            _computeProgressVisualSize();
        });

        backgroundColor = Color::gray;
        backgroundColor.forwardEmittedEvents(this);

        progressColor = Color::blue;
        progressColor.forwardEmittedEvents(this);
    }

    void ProgressBar::_computeProgressVisualSize() {
        float percentage =  static_cast<float>(value - minValue) /
                            static_cast<float>(maxValue - minValue);

        float progressWidth = getComputedSize().width * percentage;

        d_progressVisual->customWidth = static_cast<uint32_t>(progressWidth);
    }
} // namespace mc
