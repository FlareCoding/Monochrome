#pragma once
#include "Panel.h"
#include "Button.h"

namespace mc {
class ScrollPanel : public Panel {
public:
    ScrollPanel();

    PropertyObserver<bool> verticalScroll;
    PropertyObserver<bool> horizontalScroll;

    Shared<Panel>               content;

private:
    void _setupScrollPanelProperties();
    void _setupEventHandlers();
    void _createScrollbars();

    void _verifyContentAreaBounds();
    void _verifyScrollbars();

    void _verifyVerticalScrollbar();
    void _verifyHorizontalScrollbar();

    void _adjustVerticalContentPosition();
    void _adjustHorizontalContentPosition();

    Shared<Button>  d_verticalScrollbar;
    Shared<Button>  d_horizontalScrollbar;
    uint32_t        d_scrollbarThickness = 10;

    // Used in the mouseMoved handler to determine
    // if the mouse was dragging the vertical scrollbar.
    bool d_verticalScrollbarPressed = false;

    // Used in the mouseMoved handler to determine
    // if the mouse was dragging the horizontal scrollbar.
    bool d_horizontalScrollbarPressed = false;

    // Used to track position of the mouse cursor
    // when it was first pressed on with a scrollbar.
    Position d_mousePositionWhenPressed;

    // Used to track position of the vertical scrollbar
    // when it was first pressed on with a mouse.
    Position d_verticalScrollbarPositionWhenPressed;

     // Used to track position of the horizontal scrollbar
    // when it was first pressed on with a mouse.
    Position d_horizontalScrollbarPositionWhenPressed;

    // Used to track position of the panel
    // relative to the window's origin.
    Position d_positionInWindow;
};
} // namespace mc
