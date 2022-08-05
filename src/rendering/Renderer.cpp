#include "Renderer.h"
#include <core/InternalFlags.h>
#include <application/AppManager.h>

namespace mc {
    //std::pair<Position, Size> Renderer::getWidgetTruePositionAndSize(
    //    BaseWidget* widget,
    //    Position& parentPositionOffset
    //) {
    //    // Calculating frame dimensions of the widget
    //    int32_t xPos = parentPositionOffset.x + widget->position->x;
    //    int32_t yPos = parentPositionOffset.y + widget->position->y;

    //    // Calculating any necessary changes to the widget's size
    //    uint32_t width = widget->size->width;
    //    uint32_t height = widget->size->height;

    //    return {
    //        Position(xPos, yPos),
    //        Size(width, height)
    //    };
    //}

    //std::pair<Position, Size> Renderer::getWidgetTruePositionAndSize(
    //    const Shared<BaseWidget>& widget,
    //    Position& parentPositionOffset
    //) {
    //    return getWidgetTruePositionAndSize(widget.get(), parentPositionOffset);
    //}

    //void Renderer::renderScene(
    //    Color& backgroundColor,
    //    WidgetHostController& widgetHostController,
    //    Shared<RenderTarget>& renderTarget
    //) {
    //    // Render the background color layer
    //    renderTarget->clearScreen(
    //        backgroundColor.r,
    //        backgroundColor.g,
    //        backgroundColor.b,
    //        backgroundColor.a
    //    );

    //    std::vector<Shared<BaseWidget>> widgetsList(
    //        widgetHostController.getSceneWidgetList()
    //    );
    //    Position originAnchor = { 0, 0 };

    //    for (auto& widget : widgetsList) {
    //        renderWidget(renderTarget, widget, originAnchor);
    //    }
    //}

    //void Renderer::renderWidget(
    //    Shared<RenderTarget>& renderTarget,
    //    const Shared<BaseWidget>& widget,
    //    Position& parentPositionOffset
    //) {
    //    // Before anything else, check if the widget is visible
    //    if (!widget->visible) {
    //        return;
    //    }

    //    // First create a clipping layer so the contents
    //    // don't go outside the bounds of the widget.
    //    renderTarget->pushClipLayer(
    //        parentPositionOffset.x + widget->position->x,
    //        parentPositionOffset.y + widget->position->y,
    //        widget->size->width,
    //        widget->size->height
    //    );

    //    auto widgetType = widget->getType();

    //    if (widgetType == "panel") {
    //        renderPanel(
    //            renderTarget, std::static_pointer_cast<Panel>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "label") {
    //        renderLabel(
    //            renderTarget,
    //            std::static_pointer_cast<Label>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "button") {
    //        renderButton(
    //            renderTarget,
    //            std::static_pointer_cast<Button>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "checkbox") {
    //        renderCheckbox(
    //            renderTarget,
    //            std::static_pointer_cast<Checkbox>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "slider") {
    //        renderSlider(
    //            renderTarget,
    //            std::static_pointer_cast<Slider>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "entry") {
    //        renderEntry(
    //            renderTarget,
    //            std::static_pointer_cast<Entry>(widget),
    //            parentPositionOffset
    //        );
    //    } else if (widgetType == "customRenderedWidget") {
    //        renderCustomRenderedWidget(
    //            renderTarget,
    //            std::dynamic_pointer_cast<IRenderable>(widget),
    //            parentPositionOffset
    //        );
    //    }

    //    if (widget->isContainer()) {
    //        // Get the list of all children widgets and clone it
    //        std::vector<Shared<BaseWidget>> children(
    //            std::static_pointer_cast<BaseContainerWidget>(widget)->getChildren()
    //        );

    //        for (auto& child : children) {
    //            // Adjust combined parent offset to keep child
    //            // widgets positioned in relation to their parents.
    //            parentPositionOffset.x += widget->position->x;
    //            parentPositionOffset.y += widget->position->y;

    //            renderWidget(renderTarget, child, parentPositionOffset);

    //            // Restore the combined parent offset value
    //            parentPositionOffset.x -= widget->position->x;
    //            parentPositionOffset.y -= widget->position->y;
    //        }
    //    }

    //    // Restore the clipping layer
    //    renderTarget->popClipLayer();
    //}
} // namespace mc
