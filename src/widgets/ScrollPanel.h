#pragma once
#include "Panel.h"
#include "Button.h"

namespace mc {
class ScrollPanel : public Panel {
public:
    ScrollPanel();

    PropertyObserver<uint32_t> contentWidth;
    PropertyObserver<uint32_t> contentHeight;

private:
    void _setupScrollPanelProperties();
    void _setupEventHandlers();
    void _createScrollbars();
    void _verifyContentAreaBounds();
    void _verifyScrollbars();

    Shared<Button>  d_verticalScrollbar;
    Shared<Button>  d_horizontalScrollbar;
    uint32_t        d_scrollbarThickness = 10;

    Position d_contentOrigin = { 0, 0 };

    // Used in the mouseMoved handler to determine
    // if the mouse was dragging the scrollbar.
    bool d_mousePressed = false;

    // Used to track position of the mouse cursor
    // when it was first pressed on with a scrollbar.
    Position d_mousePositionWhenPressed;

    // Used to track position of the scrollbar
    // when it was first pressed on with a mouse.
    Position d_verticalScrollbarPositionWhenPressed;

    // Used to track position of the panel
    // relative to the window's origin.
    Position d_positionInWindow;
};
} // namespace mc
