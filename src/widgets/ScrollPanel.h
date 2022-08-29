#pragma once
#include "Button.h"
#include "visuals/RectVisual.h"

namespace mc {
class ScrollPanel : public BaseContainerWidget {
public:
    ScrollPanel();

    PropertyObserver<uint32_t>  cornerRadius;
    PropertyObserver<Color>     backgroundColor;

    void scrollContentVertically(int32_t amount);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    Shared<RectVisual> d_bodyVisual;

    Shared<Button> d_verticalTrackUpButton;
    Shared<Button> d_verticalTrackDownButton;
    Shared<Button> d_verticalScrollbar;

    void _clampContentPosition();

    uint32_t _calculateVerticalScrollbarSize();
    void _calculateVerticalScrollbarPosition();

private:
    bool        d_mousePressed = false;
    Position    d_mousePressLocation = Point(0, 0);
    Position    d_positionInWindow = Point(0, 0);
    int32_t     d_preScrollContentPosition = 0;

    void _verticalScrollbarOnMouseDown(Shared<Event> e);
    void _verticalScrollbarOnMouseUp(Shared<Event> e);
    void _verticalScrollbarOnMouseMoved(Shared<Event> e);

    void _showVerticalScrollElements();
    void _hideVerticalScrollElements();

private:
    const uint64_t d_privateWidgets = 3;
    const uint32_t d_scrollbarTrackSize = 14;
};
} // namespace mc
