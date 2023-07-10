#include "EditorCanvasController.h"

namespace mc::mcstudio {
    EditorCanvasController::EditorCanvasController(Shared<Canvas2D> overlayCanvas) {
        d_canvas = overlayCanvas;
    }

    void EditorCanvasController::clearCanvas() {
        d_canvas->clear();
    }

    void EditorCanvasController::drawWidgetEditFrame(Shared<BaseWidget> widget) {
        auto widgetAbsPosition = widget->getPositionInWindow();
        auto widgetSize = widget->getComputedSize();

        // Draw the rectangular frame around the widget
        d_canvas->strokeRectangle(
            widgetAbsPosition,
            widgetSize,
            widgetEditFrameColor
        );

        // Draw the widget's name/type above the editor frame
        widgetAbsPosition.y -= 20;
        widgetSize.height = 20;
        d_canvas->fillText(
            widgetAbsPosition,
            widgetSize,
            widgetEditFrameColor,
            widget->getWidgetName(),
            "Verdana",
            14,
            "normal",
            "left"
        );
    }
} // namespace mc::mcstudio
