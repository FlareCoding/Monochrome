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

private:
    static void renderWidget(
        Shared<RenderTarget>& renderTarget,
        Shared<BaseWidget> widget,
        Position parentOffset
    );

    static void drawVisualElement(
        Shared<RenderTarget>& renderTarget,
        Shared<VisualElement> visual,
        Position& parentOffset
    );

    static void drawRectVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<RectVisual> visual,
        Position& parentOffset
    );

    static void drawBorderVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<BorderVisual> visual,
        Position& parentOffset
    );

    /*static void drawCircleVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<BorderVisual> visual,
        Position& parentOffset
    );*/

    static void drawTextVisual(
        Shared<RenderTarget>& renderTarget,
        Shared<TextVisual> visual,
        Position& parentOffset
    );
};
} // namespace mc
