#pragma once
#include "BaseWidget.h"
#include "layouts/Layout.h"

namespace mc {
class StackPanel : public BaseContainerWidget {
public:
    StackPanel();

    // Radius of the panel's corners
    PropertyObserver<uint32_t>      cornerRadius;

    // Color of the panel's body
    PropertyObserver<Color>         backgroundColor;

    // Direction in which the content
    // will be layed out inside the panel.
    PropertyObserver<Orientaion>    orientation;

    Size updateLayout() override;

protected:
    Size _measureSize() override;

private:
    void _createVisuals();
    void _setupProperties();
};
} // namespace mc
