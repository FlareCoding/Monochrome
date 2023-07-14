#include "Renderer.h"
#include <core/InternalFlags.h>
#include <application/AppManager.h>
#include <widgets/BaseWidget.h>

namespace mc {
    using WidgetList_t = std::vector<Shared<BaseWidget>>;
    static WidgetList_t cloneWidgetList(const WidgetList_t& original) {
        WidgetList_t clone(original);
        return clone;
    }

    bool Renderer::enableDebugBoundingBoxes = false;

    void Renderer::renderScene(
        const Color& backgroundColor,
        Shared<BaseWidget> rootContainer,
        Shared<RenderTarget>& renderTarget
    ) {
        // Clear the screen only if the root container is dirty and
        // needs redrawing, or if there is no root container at all.
        if (!rootContainer.get() || rootContainer->isPaintDirty()) {
            renderTarget->clearScreen(
                backgroundColor.r,
                backgroundColor.g,
                backgroundColor.b,
                backgroundColor.a
            );
        }

        if (rootContainer.get()) {
            renderWidget(renderTarget, rootContainer, { 0, 0 });
        }
    }

    void Renderer::renderWidget(
        Shared<RenderTarget>& renderTarget,
        Shared<BaseWidget> widget,
        Position parentOffset
    ) {
        if (!widget->visible.get()) {
            return;
        }

        Position widgetPosition = parentOffset + widget->position;
        Size widgetSize = widget->getComputedSize();

        // Create a clipping layer so the contents
        // don't go outside the bounds of the widget.
        renderTarget->pushClipLayer(
            widgetPosition.x,
            widgetPosition.y,
            widgetSize.width,
            widgetSize.height
        );

        // First draw the core visual elements
        // that the widget is comprised from.
        if (widget->isPaintDirty()) {
            if (hasTransparentVisual(widget.get())) {
                redrawParentVisualForChildClippedRegion(renderTarget,
                    widget.get(), parentOffset);
            }

            drawVisualElementList(renderTarget,
                widget->d_coreVisualElements, widgetPosition, widgetSize);
        }

        // Render all child elements
        for (auto& child : cloneWidgetList(widget->_getChildren())) {
            renderWidget(renderTarget, child, widgetPosition);
        }

        // Lastly draw the overlay visual
        // elements that the widget has.
        if (widget->isPaintDirty()) {
            drawVisualElementList(renderTarget,
                widget->d_overlayVisualElements, widgetPosition, widgetSize);

            widget->d_isPaintDirty = false;
        }

        // Restore the clipping layer
        renderTarget->popClipLayer();

        if (enableDebugBoundingBoxes) {
            renderTarget->drawRectangle(
                widgetPosition.x,
                widgetPosition.y,
                widgetSize.width,
                widgetSize.height,
                Color(180, 0, 0, 255),
                0, false, 2
            );
        }
    }

    void Renderer::redrawParentVisualForChildClippedRegion(
        Shared<RenderTarget>& renderTarget,
        BaseWidget* childWidget,
        Position parentOffset
    ) {
        auto parent = childWidget->getParent();
        if (!parent) {
            return;
        }

        auto parentSize = parent->getComputedSize();

        // If the parent also has transparency, redraw its parent first
        if (hasTransparentVisual(parent)) {
            auto ancestorOffset = parentOffset - parent->position;

            redrawParentVisualForChildClippedRegion(renderTarget, parent, ancestorOffset);
        }

        drawVisualElementList(renderTarget,
            parent->d_coreVisualElements, parentOffset, parentSize);

        drawVisualElementList(renderTarget,
            parent->d_overlayVisualElements, parentOffset, parentSize);
    }

    void Renderer::drawVisualElementList(
        Shared<RenderTarget>& renderTarget,
        std::vector<Shared<VisualElement>>& visualElements,
        Position& widgetPosition,
        Size& widgetSize
    ) {
        uint64_t clipRegions = 0;

        for (auto& visual : visualElements) {
            // Special handling for clip region elements
            if (visual->type() == VisualTypeClipRegion) {
                auto clipRegion = std::static_pointer_cast<ClipRegionVisual>(visual);

                // Push the clip layer
                renderTarget->pushClipLayer(
                    widgetPosition.x + clipRegion->position->x,
                    widgetPosition.y + clipRegion->position->y,
                    clipRegion->customWidth, clipRegion->customHeight
                );

                // Keep track of how many clip regions were pushed to pop them
                // after all other visuals finish rendering for the current widget.
                ++clipRegions;
                continue;
            }

            if (!visual->visible) {
                continue;
            }

            auto visualSize = widgetSize;
            if (visual->customWidth != NOT_SET) {
                visualSize.width = visual->customWidth;
            }

            if (visual->customHeight != NOT_SET) {
                visualSize.height = visual->customHeight;
            }

            drawVisualElement(renderTarget, visual, widgetPosition, visualSize);
        }

        // Pop the clip layers if there were any
        while (clipRegions) {
            renderTarget->popClipLayer();
            --clipRegions;
        }
    }

    void Renderer::drawVisualElement(
        Shared<RenderTarget>& renderTarget,
        Shared<VisualElement> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
        if (visual->color->isTransparent()) {
            return;
        }

        switch (visual->type()) {
        case VisualType::VisualTypeRect: {
            drawRectVisual(
                renderTarget,
                std::static_pointer_cast<RectVisual>(visual),
                parentOffset,
                visualSize
            );
            break;
        }
        case VisualType::VisualTypeBorder: {
            drawBorderVisual(
                renderTarget,
                std::static_pointer_cast<BorderVisual>(visual),
                parentOffset,
                visualSize
            );
            break;
        }
        case VisualType::VisualTypeCircle: {
            break;
        }
        case VisualType::VisualTypeText: {
            drawTextVisual(
                renderTarget,
                std::static_pointer_cast<TextVisual>(visual),
                parentOffset,
                visualSize
            );
            break;
        }
        case VisualType::VisualTypeImage: {
            drawImageVisual(
                renderTarget,
                std::static_pointer_cast<ImageVisual>(visual),
                parentOffset,
                visualSize
            );
            break;
        }
        default: {
            CORE_ASSERT(false, "Attempted to render an unknown visual type");
            break;
        }
        }
    }

    void Renderer::drawRectVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<RectVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
        renderTarget->drawRectangle(
            parentOffset.x + visual->position->x,
            parentOffset.y + visual->position->y,
            visualSize.width,
            visualSize.height,
            visual->color,
            visual->cornerRadius,
            true,
            0
        );
    }

    void Renderer::drawBorderVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<BorderVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
        renderTarget->drawRectangle(
            parentOffset.x + visual->position->x,
            parentOffset.y + visual->position->y,
            visualSize.width,
            visualSize.height,
            visual->color,
            visual->cornerRadius,
            false,
            visual->thickness
        );
    }

    void Renderer::drawTextVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<TextVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
        if (visual->text->empty()) {
            return;
        }

        renderTarget->drawText(
            parentOffset.x + visual->position->x,
            parentOffset.y + visual->position->y,
            visualSize.width,
            visualSize.height,
            visual->color,
            visual->text,
            visual->font,
            visual->fontSize,
            visual->fontStyle,
            visual->alignment,
            visual->wordWrapMode
        );
    }

    void Renderer::drawImageVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<ImageVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
        // Make sure the image bitmap is present and valid
        if (!visual->imageBitmap) {
            return;
        }

        renderTarget->drawBitmap(
            parentOffset.x + visual->position->x,
            parentOffset.y + visual->position->y,
            visualSize.width,
            visualSize.height,
            visual->imageBitmap,
            visual->opacity
        );
    }

    bool Renderer::hasTransparentVisual(BaseWidget* widget) {
        // First check if the widget has core visuals at all
        if (widget->d_coreVisualElements.empty()) {
            return true;
        }

        // Check core visual elements
        for (auto& visualElement : widget->d_coreVisualElements) {
            if (!visualElement->visible.get()) {
                continue;
            }

            if (visualElement->color->hasTransparency()) {
                return true;
            }
        }

        // Check overlay visual elements
        for (auto& visualElement : widget->d_overlayVisualElements) {
            if (!visualElement->visible.get()) {
                continue;
            }

            if (visualElement->color->hasTransparency()) {
                return true;
            }
        }

        return false;
    }
} // namespace mc
