#include "Renderer.h"
#include <core/InternalFlags.h>
#include <application/AppManager.h>
#include <widgets/BaseWidget.h>

namespace mc {
    void Renderer::renderScene(
        const Color& backgroundColor,
        Shared<BaseWidget> rootContainer,
        Shared<RenderTarget>& renderTarget
    ) {
        renderTarget->clearScreen(
            backgroundColor.r,
            backgroundColor.g,
            backgroundColor.b,
            backgroundColor.a
        );

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
        Size widgetSize = widget->getClientSize();

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
        for (auto& visual : widget->d_coreVisualElements) {
            if (!visual->visible) {
                continue;
            }

            Size visualSize = { visual->width, visual->height };
            if (visualSize.width == NOT_SET && visualSize.height == NOT_SET) {
                visualSize = { widget->width, widget->height };
            }

            drawVisualElement(renderTarget, visual, widgetPosition, visualSize);
        }

        // Render all child elements
        for (auto& child : widget->_getChildren()) {
            renderWidget(renderTarget, child, widgetPosition);
        }

        // Lastly draw the overlay visual
        // elements that the widget has.
        for (auto& visual : widget->d_overlayVisualElements) {
            if (!visual->visible) {
                continue;
            }

            Size visualSize = { visual->width, visual->height };
            if (visualSize.width == NOT_SET && visualSize.height == NOT_SET) {
                visualSize = { widget->width, widget->height };
            }

            drawVisualElement(renderTarget, visual, widgetPosition, visualSize);
        }

        // Restore the clipping layer
        renderTarget->popClipLayer();
    }

    void Renderer::drawVisualElement(
        Shared<RenderTarget>& renderTarget,
        Shared<VisualElement> visual,
        Position& parentOffset,
        const Size& visualSize
    ) {
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
} // namespace mc
