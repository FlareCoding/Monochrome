#pragma once
#include "UIWindow.h"
#include <widgets/BaseWidget.h>

namespace mc {
class Overlay {
public:
    Overlay();
    ~Overlay();

    void setSize(uint32_t width, uint32_t height);
    void setSize(const Size& size);
    void show();
    void hide();
    void setContent(Shared<BaseWidget> content);

    Color getBackgroundColor();
    void setBackgroundColor(const Color& color);

    void setAnchor(const Point& point);
    void setActivatorWidget(BaseWidget* widget);

    // Specifies the preferred orientation for spawning
    // the overlay window around the activator widget.
    OverflowDirection   spawnDirection;

    // Specifies if the overlay should automatically
    // hide whenever the overlay window loses focus.
    bool                autoHide;

    // Specifies a child overlay so that when
    // the mouse is clicked onto the child, the
    // parent overlay doesn't automatically close.
    void addChildOverlay(Shared<Overlay> overlay);

private:
    Shared<UIWindow>    d_overlayWindow;

    BaseWidget*         d_activatorWidget = nullptr;
    Point               d_anchorPoint = { 0, 0 };
    bool                d_overlayOpened = false;

    std::vector<Shared<Overlay>> d_childOverlays;

    Position            _calculateAnchorPosition(Shared<MouseButtonEvent> e);
    bool                _isMouseClickedInOverlay(const Position& clickPosition);
};
} // namespace mc
