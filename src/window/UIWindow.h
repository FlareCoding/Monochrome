#pragma once
#include "NativeWindow.h"
#include <utils/uuid.h>
#include <thread>
#include <atomic>
#include "EventProcessor.h"
#include "Canvas2D.h"

namespace mc {
class BaseContainerWidget;

class UIWindow : public EventEmitter {
public:
    UIWindow(
        uint64_t windowFlags,
        uint32_t width = 800,
        uint32_t height = 600,
        const std::string& title = "Window"
    );
    virtual ~UIWindow();

    uint32_t getWidth();
    uint32_t getHeight();
    const std::string& getTitle();
    Position getPosition();
    Size getSize();
    void setWidth(uint32_t width);
    void setHeight(uint32_t height);
    void setSize(uint32_t width, uint32_t height);
    void setMinWidth(uint32_t minWidth);
    void setMaxWidth(uint32_t maxWidth);
    void setMinHeight(uint32_t minHeight);
    void setMaxHeight(uint32_t maxHeight);
    void setPosition(const Position& pos);
    virtual void setTitle(const std::string& title);
    bool isFocused() const;

    void show();
    void hide();
    void focus();
    void unfocus();
    void close();
    void maximize();
    void restoreMaximize();
    void minimize();

    void update();

    void setShouldRedraw();
    inline bool shouldRedraw() const { return d_shouldRedrawScene; }

    Color getBackgroundColor() const { return d_backgroundColor; }
    virtual void setBackgroundColor(const Color& color) { d_backgroundColor = color; }

    inline Shared<BaseContainerWidget> getRootWidget() const { return d_rootWidget; }
    void setRootWidget(Shared<BaseContainerWidget> root);

    Shared<Canvas2D> createOverlayCanvas();
    inline Shared<Canvas2D> getOverlayCanvas() { return d_overlayCanvas; }

private:
    Shared<NativeWindow> d_nativeWindow = nullptr;
    Shared<BaseContainerWidget> d_rootWidget;
    Shared<EventProcessor> d_eventProcessor;

    // Can be created optionally by the user
    Shared<Canvas2D> d_overlayCanvas = nullptr;

    uuid_t d_uuid = 0;
    std::atomic_bool d_isDestroyed = false;
    Color d_backgroundColor = Color::darkGray;

    std::thread d_renderingThread;

    std::atomic_bool d_shouldRedrawScene = true;
    std::atomic_bool d_onDemandBufferSwapRequested = false;

    void _backgroundRenderingTask();
    void _renderScene(Shared<RenderTarget>& renderTarget);

    inline void _requestOnDemandBufferSwap() { d_onDemandBufferSwapRequested = true; }
    inline void _completeOnDemandBufferSwap() { d_onDemandBufferSwapRequested = false; }
    bool _shouldSwapBuffersOnDemand() const { return d_onDemandBufferSwapRequested; }
};
} // namespace mc
