#pragma once
#include "BaseWidget.h"

namespace mc {
class StackPanel : public BaseContainerWidget {
public:
    StackPanel();

    std::string getWidgetName() const override { return "StackPanel"; }

    // Radius of the panel's corners
    PropertyObserver<uint32_t>      cornerRadius;

    // Color of the panel's body
    PropertyObserver<Color>         backgroundColor;

    // Direction in which the content
    // will be layed out inside the panel.
    PropertyObserver<Orientation>    orientation;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

    void _finalizeChild(
        Shared<BaseWidget> child,
        const Size& contentSize,
        const Position& childSlotPosition,
        uint32_t growableSpace
    );

private:
    void _createVisuals();
    void _setupProperties();

private:
    uint32_t totalLayoutWeight = 0;
};
} // namespace mc
