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

private:
    void _createVisuals();
    void _setupProperties();

    void _onChildAdded(BaseWidget* child);
    void _onChildRemoved(BaseWidget* child);

    void _layoutUpdate();

private:
    Shared<Layout> d_verticalLayout;
    Shared<Layout> d_horizontalLayout;
};
} // namespace mc
