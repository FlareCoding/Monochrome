#pragma once
#include <window/NativeWindow.h>
#include "Win32RenderTarget.h"
#include <Windows.h>
#include <widgets/Layer.h>

namespace mc {
class Win32NativeWindow : public NativeWindow {
public:
    explicit Win32NativeWindow(uint64_t windowFlags);
    ~Win32NativeWindow();

    uint32_t getWidth() const { return d_width; }
    uint32_t getHeight() const { return d_height; }
    Position getPosition() const { return d_position; }
    Size     getSize() const { return Size(d_width, d_height); }
    const std::string& getTitle() const { return d_title; }

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

    void requestRedraw() override;

    Shared<RenderTarget> getRenderTarget() const { return d_renderTarget; }

    //
    // Win32NativeWindow specific functionality
    //
    void destroyPlatformWindow();

    LRESULT CALLBACK win32WindowProcCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    inline bool isFrontBufferRenderRequested() const { return d_requestedFrontBufferRender; }
    inline void requestFrontBufferRender() { d_requestedFrontBufferRender = true; }
    inline void completeFrontBufferRender() { d_requestedFrontBufferRender = false; }

private:
    bool        d_visible = false;
    bool        d_focused = false;
    uint32_t    d_width = 0;
    uint32_t    d_height = 0;
    Size        d_minSize = { 460, 60 };
    Size        d_maxSize = { 6500, 6500 };
    Position    d_position = { 0, 0 };
    float       d_scalingFactor = 1;
    std::string d_title;
    Position    d_previousMousePosition;
    uint64_t    d_windowFlags = 0;
    bool        d_mouseInClientArea = false;
    bool        d_requestedFrontBufferRender = false;

    HWND d_windowHandle = nullptr;
    Shared<Win32RenderTarget> d_renderTarget = nullptr;

    void _createWin32Window(uint64_t windowFlags);
    void _setWindowSize(uint32_t width, uint32_t height);
};
} // namespace mc
