#pragma once
#include "Label.h"
#include "visuals/RectVisual.h"
#include "visuals/BorderVisual.h"

namespace mc {
class ProgressBar : public BaseWidget {
public:
    ProgressBar();

    PropertyObserver<Color>       backgroundColor;
    PropertyObserver<Color>       progressColor;
    PropertyObserver<int32_t>     value;
    PropertyObserver<int32_t>     maxValue;
    PropertyObserver<int32_t>     minValue;

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual> d_bodyVisual;
    Shared<RectVisual> d_progressVisual;
};
} // namespace mc
