#pragma once
#include "BaseWidget.h"

namespace mc {
class StackPanel : public BaseContainerWidget {
public:
    StackPanel();

    // Radius of the panel's corners
    PropertyObserver<uint32_t>      cornerRadius;

    // Color of the panel's body
    PropertyObserver<Color>         backgroundColor;

private:
    void _createVisuals();
    void _setupProperties();

    void _onChildAdded(BaseWidget* child);
    void _onChildRemoved(BaseWidget* child);

private:
    Position nextAvailableCellPosition = { 0, 0 };
};
} // namespace mc
