#pragma once
#include <widgets/AllWidgets.h>
#include <window/Canvas2D.h>

namespace mc::mcstudio {
class EditorCanvasController {
public:
    EditorCanvasController(Shared<Canvas2D> overlayCanvas);

    void clearCanvas();

    void drawWidgetEditFrame(Shared<BaseWidget> widget);

    Color widgetEditFrameColor = Color::red;

private:
    Shared<Canvas2D> d_canvas;
};
} // namespace mc::mcstudio
