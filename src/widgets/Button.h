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

    Shared<Label> label;

protected:
    Size _measureSize() override;
    void _onSetComputedSize(const Size& size) override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual> d_bodyVisual;
    Shared<BorderVisual> d_borderVisual;
};
} // namespace mc
