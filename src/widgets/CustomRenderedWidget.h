#pragma once
#include "BaseContainerWidget.h"

namespace mc {

class RenderTarget;

using getWidgetBoundsFn_t =
    std::function<std::pair<Position, Size>(
                    BaseWidget* widget,
                    Position& parentPositionOffset
                )>;

/**
 * @brief IRenderable interface allows the Renderer
 *        to execute custom drawing logic on more complex
 *        widgets that require to draw custom shapes.
 */
class IRenderable {
public:
    /**
     * @brief Called during the scene rendering routine.
     *        This function must be used by classes that
     *        implement the IRenderable interface to perform
     *        their custom drawing logic for the widget.
     *        *Note* this function is called by the Renderer
     *               only after the clipping region has been pushed.
     * 
     * @param renderTarget Drawing context provided by the renderer
     *                     when the routine is called.
     * 
     * @param parentPositionOffset Positional offset from the window's origin.
     * 
     * @param getWidgetBounds Optional function that the renderer will provide
    *                         to calculate widget's true bounds including parent
     *                        positional offset, margins, and widget's own size.
     *                               
     */
    virtual
    void onRender(
        Shared<RenderTarget>& renderTarget,
        Position& parentPositionOffset,
        getWidgetBoundsFn_t getWidgetBounds
    ) = 0;
};

class CustomRenderedWidget
    : public BaseWidget, public IRenderable {
public:
    CustomRenderedWidget() : BaseWidget() {}
    inline const std::string getType() const override { return "customRenderedWidget"; }

private:
    void _setupProperties() override {}
};

class CustomRenderedContainerWidget
    : public BaseContainerWidget, public IRenderable {
public:
    CustomRenderedContainerWidget() : BaseContainerWidget() {}
    inline const std::string getType() const override { return "customRenderedWidget"; }

private:
    void _setupProperties() override {}
};
} // namespace mc
