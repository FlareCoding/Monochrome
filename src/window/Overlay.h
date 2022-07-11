#pragma once
#include "UIWindow.h"
#include <widgets/BaseWidget.h>

namespace mc
{
    class Overlay
    {
    public:
        Overlay();
        ~Overlay();

        void setSize(uint32_t width, uint32_t height);
        void show();
        void hide();
        void setContent(Shared<BaseWidget> content);

        Color getBackgroundColor();
        void setBackgroundColor(const Color& color);

        void setAnchor(const Point& point);
        void setActivatorWidget(Shared<BaseWidget> widget);

        // Specifies the preferred orientation for spawning
        // the overlay window around the activator widget.
        OverflowDirection   spawnDirection;

        // Specifies if the overlay should automatically
        // hide whenever the overlay window loses focus.
        bool                autoHide;

    private:
        Shared<UIWindow>    d_overlayWindow;

        Shared<BaseWidget>  d_activatorWidget = nullptr;
        Point               d_anchorPoint = { 0, 0 };

        Position            _calculateAnchorPosition(Shared<MouseButtonEvent> e);
    };
}
