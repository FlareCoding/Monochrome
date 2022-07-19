#pragma once
#include "CustomRenderedWidget.h"

namespace mc {
class WidgetControlFrame : public CustomRenderedWidget {
public:
    WidgetControlFrame();

    // The widget to be surrounded by a resizable frame
    // and can be dragged with a mouse if widgetDraggable is true.
    PropertyObserver<Shared<BaseWidget>> content;

    // Determines if the frame should be
    // displayed and can be interacted with.
    PropertyObserver<bool> enabled;

    // Determines if the widget can be dragged around with a mouse
    PropertyObserver<bool> widgetDraggable;

    // Color of the resize anchor square that the user drags
    PropertyObserver<Color> resizeAnchorColor;

    void onRender(
        Shared<RenderTarget>& renderTarget,
        Position& parentPositionOffset,
        getWidgetBoundsFn_t getWidgetBounds
    ) override;

private:
    const int32_t d_anchorSquareSize = 5;
    const int32_t d_minAcceptableWidth = 40;
    const int32_t d_minAcceptableHeight = 20;

    // Indicates which edge of the frame the mouse
    // was anchored to when it was first pressed down.
    // Note: 0 indicates invalid anchor.
    int d_resizeAnchor = 0;

    // Indicates whether the mouse was inside
    // the frame when it was first pressed down.
    bool d_mousePressed = false;

    // When the mouse is moved after being pressed
    // on the frame, where the mouse was first pressed
    // down becomes the mouse anchor position.
    Position d_mouseAnchorPosition;

    void _setupEventHandlers();

    void _onMouseDown(const Position& mousePos);
    void _onMouseMoved(const Position& mousePos);
    void _onMouseUp();

    int _getAnchorFromPosition(const Position& pos);
};
} // namespace mc
