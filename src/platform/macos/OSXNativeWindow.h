#pragma once
#include <window/NativeWindow.h>
#include "OSXRenderTarget.h"
#include "OSXApplicationContext.h"

namespace mc { class OSXNativeWindow; }

@interface OSXWindowDelegate : NSWindow
@property (nonatomic, assign) mc::OSXNativeWindow* mcWindowHandle;
@end

@interface OSXWindowContentViewDelegate : NSView
@property (nonatomic, assign) mc::OSXNativeWindow* mcWindowHandle;
- (BOOL)isFlipped;
- (void)drawRect:(NSRect)rect;
- (void)update;
@end

@interface OSXWindowClass : NSWindow
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
@end

namespace mc {
class OSXNativeWindow : public NativeWindow {
public:
    explicit OSXNativeWindow(uint64_t windowFlags);
    ~OSXNativeWindow();

    uint32_t getWidth() const { return d_width; }
    uint32_t getHeight() const { return d_height; }
    Position getPosition() const { return d_position; }
    Size     getSize() const { return Size(d_width, d_height); }
    const std::string& getTitle() const { return d_title; }

    Size     getMinSize() const { return d_minSize; }
    Size     getMaxSize() const { return d_maxSize; }

    void setWidth(uint32_t width);
    void setHeight(uint32_t height);
    void setMinWidth(uint32_t minWidth);
    void setMaxWidth(uint32_t maxWidth);
    void setMinHeight(uint32_t minHeight);
    void setMaxHeight(uint32_t maxHeight);
    void setPosition(const Position& pos);
    void setTitle(const std::string& title);

    bool isFocused() const { return d_focused; }

    void show();
    void hide();
    void focus();
    void unfocus();
    void close();
    void maximize();
    void restoreMaximize();
    void minimize();

    void requestRedraw();

    Shared<RenderTarget> getRenderTarget() const { return d_renderTarget; }

    uint64_t& getWindowFlags() { return d_windowFlags; }

    Shared<OSXApplicationContext> getApplicationContext() const;
    void updatePlatformWindow();

    inline bool isFrontBufferRenderRequested() const { return d_requestedFrontBufferRender; }
    inline void requestFrontBufferRender() { d_requestedFrontBufferRender = true; }
    inline void completeFrontBufferRender() { d_requestedFrontBufferRender = false; }

private:
    bool        d_visible = false;
    bool        d_focused = false;
    Position    d_position = { 0, 0 };
    uint32_t    d_width = 0;
    uint32_t    d_height = 0;
    Size        d_minSize = { 460, 60 };
    Size        d_maxSize = { 6500, 6500 };
    float       d_scalingFactor = 1;
    std::string d_title;
    uint64_t    d_windowFlags = 0;
    bool        d_requestedFrontBufferRender = false;

    NSWindow*   d_windowHandle = nullptr;
    Shared<OSXRenderTarget> d_renderTarget = nullptr;

    void _createOSXWindow(uint64_t windowFlags);
    void _setWindowSize(uint32_t width, uint32_t height);
};
} // namespace mc
