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
        if (widgetSize.width < 200) {
            widgetSize.width = 200;
        }

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

        /*
        * 
        *  THE CANVAS API IS CURRENTLY IN DEVELOPMENT AND SHOULD NOT BE USED.
        *  THE ISSUE IS PLANNED AND EXPECTED TO BE ADDRESSED IN COMING UDPATES.
        * 
        */
    }
} // namespace mc::mcstudio
