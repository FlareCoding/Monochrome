#pragma once
#include "RenderTarget.h"
#include <widgets/WidgetContainer.h>
#include <widgets/Layer.h>
#include <widgets/visuals/Visuals.h>

namespace mc {
class Renderer {
public:
    static void renderScene(
        const Color& backgroundColor,
        Shared<BaseWidget> rootContainer,
        Shared<RenderTarget>& renderTarget
    );

    static bool enableDebugBoundingBoxes;

private:
    static void renderWidget(
        Shared<RenderTarget>& renderTarget,
        Shared<BaseWidget> widget,
        Position parentOffset
    );

    static void drawVisualElementList(
        Shared<RenderTarget>& renderTarget,
        std::vector<Shared<VisualElement>>& visualElements,
        Position& widgetPosition,
        Size& widgetSize
    );

    static void drawVisualElement(
        Shared<RenderTarget>& renderTarget,
        Shared<VisualElement> visual,
        Position& parentOffset,
        const Size& visualSize
    );

    static void drawRectVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<RectVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    );

    static void drawBorderVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<BorderVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    );

    /*static void drawCircleVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<BorderVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    );*/

    static void drawTextVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<TextVisual> visual,
        Position& parentOffset,
        const Size& visualSize
    );
};
} // namespace mc
