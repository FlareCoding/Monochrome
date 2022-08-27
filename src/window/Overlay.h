#pragma once
#include "UIWindow.h"
#include  <events/MouseEvents.h>
//#include <widgets/BaseWidget.h>

namespace mc {
enum OverlayActivationType : uint8_t {
    OnHover,
    OnClick
};

class Overlay {
public:
    Overlay();
    ~Overlay();

    void setSize(uint32_t width, uint32_t height);
    void setSize(const Size& size);
    Size getSize();
    void show();
    void hide();

    Color getBackgroundColor();
    void setBackgroundColor(const Color& color);

    void setAnchor(const Point& point);

    void setActivatorWidget(BaseWidget* widget, OverlayActivationType activationType);
    inline BaseWidget* getActivatorWidget() const { return d_activatorWidget; }

    // Closes all child overlays
    void closeChildOverlays();

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

    // Sets the root widget of the underlying UIWindow instance
    void setContent(Shared<BaseContainerWidget> root);

private:
    Shared<UIWindow>    d_overlayWindow;

    BaseWidget*         d_activatorWidget = nullptr;
    Point               d_anchorPoint = { 0, 0 };
    bool                d_overlayOpened = false;

    std::vector<Shared<Overlay>> d_childOverlays;

    Position            _calculateAnchorPosition(Shared<MouseMovedEvent> e);
    bool                _isMouseClickedInOverlay(const Position& clickPosition);
    void                _hideThisAndChildOverlays();
};
} // namespace mc
