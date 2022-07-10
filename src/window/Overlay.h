#pragma once
#include "UIWindow.h"
#include <widgets/BaseWidget.h>

namespace mc
{
    class Overlay : public EventEmitter
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

        bool autoHide = true;

    private:
        Shared<UIWindow>    d_overlayWindow;

        Shared<BaseWidget>  d_activatorWidget = nullptr;
        Point               d_anchorPoint = { 0, 0 };
    };
}
