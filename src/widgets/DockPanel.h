#pragma once
#include "BaseWidget.h"

namespace mc {
class DockPanel : public BaseContainerWidget {
public:
    DockPanel();

    // @brief Radius of the panel's corners
    PropertyObserver<uint32_t> cornerRadius;

    // @brief Background color of the remaining space in the panel
    PropertyObserver<Color> backgroundColor;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    void _finalizeChild(
        Shared<BaseWidget> child,
        Frame& availableSpace
    );
};
} // namespace mc
