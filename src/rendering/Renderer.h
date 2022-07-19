#pragma once
#include <widgets/WidgetHostController.h>
#include "RenderTarget.h"
#include <widgets/Panel.h>
#include <widgets/Label.h>
#include <widgets/Button.h>
// #include <widgets/Checkbox.h>
// #include <widgets/Slider.h>
#include <widgets/Entry.h>
#include <widgets/CustomRenderedWidget.h>

namespace mc {
class Renderer {
public:
    static void renderScene(
        Color& backgroundColor,
        WidgetHostController& widgetHostController,
        Shared<RenderTarget>& renderTarget
    );

    // Renders the widget in the current graphics context
    // @param renderTarget Render target object that holds the graphics context
    // @param widget Widget instance that needs to be rendered
    // @param parentPositionOffset Combined offset of all
    //        parents so that the child is rendered in relation to its parent.
    static void renderWidget(
        Shared<RenderTarget>& renderTarget,
        const Shared<BaseWidget>& widget,
        Position& parentPositionOffset
    );

private:
    static std::pair<Position, Size> getWidgetTruePositionAndSize(
        BaseWidget* widget,
        Position& parentPositionOffset
    );

    static std::pair<Position, Size> getWidgetTruePositionAndSize(
        const Shared<BaseWidget>& widget,
        Position& parentPositionOffset
    );

    static void renderPanel(
        Shared<RenderTarget>& renderTarget,
        const Shared<Panel>& panel,
        Position& parentPositionOffset
    );

    static void renderLabel(
        Shared<RenderTarget>& renderTarget,
        const Shared<Label>& label,
        Position& parentPositionOffset
    );

    static void renderButton(
        Shared<RenderTarget>& renderTarget,
        const Shared<Button>& button,
        Position& parentPositionOffset
    );

    // static void renderCheckbox(
    //     Shared<RenderTarget>& renderTarget,
    //     const Shared<Checkbox>& checkbox,
    //     Position& parentPositionOffset
    // );

    // static void renderSlider(
    //     Shared<RenderTarget>& renderTarget,
    //     const Shared<Slider>& slider,
    //     Position& parentPositionOffset
    // );

        static void renderEntry(
            Shared<RenderTarget>& renderTarget,
            const Shared<Entry>& entry,
            Position& parentPositionOffset
        );

        static void renderCustomRenderedWidget(
            Shared<RenderTarget>& renderTarget,
            const Shared<IRenderable>& renderable,
            Position& parentPositionOffset
        );
};
} // namespace mc
