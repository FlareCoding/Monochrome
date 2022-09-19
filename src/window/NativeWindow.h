#pragma once
#include <application/AppManager.h>
#include <events/EventEmitter.h>
#include <rendering/RenderTarget.h>
#include <widgets/Layer.h>

namespace mc {
class NativeWindow : public EventEmitter {
public:
    static Shared<NativeWindow> create(bool isRoot, uint64_t windowFlags);

    inline bool isRoot() const { return d_isRoot; }

    void setUpdateCallback(std::function<void()> callback) { d_updateCallback = callback; }
    std::function<void()> getUpdateCallback() const { return d_updateCallback; }

    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual Position getPosition() const = 0;
    virtual Size getSize() const = 0;
    virtual const std::string& getTitle() const = 0;
    virtual void setWidth(uint32_t width) = 0;
    virtual void setHeight(uint32_t height) = 0;
    virtual void setMinWidth(uint32_t minWidth) = 0;
    virtual void setMaxWidth(uint32_t maxWidth) = 0;
    virtual void setMinHeight(uint32_t minHeight) = 0;
    virtual void setMaxHeight(uint32_t maxHeight) = 0;
    virtual void setPosition(const Position& pos) = 0;
    virtual void setTitle(const std::string& title) = 0;
    virtual bool isFocused() const = 0;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual void close() = 0;
    virtual void maximize() = 0;
    virtual void restoreMaximize() = 0;
    virtual void minimize() = 0;

    virtual void requestRedraw() = 0;

    virtual Shared<RenderTarget> getRenderTarget() const = 0;

    virtual bool isFrontBufferRenderRequested() const = 0;
    virtual void requestFrontBufferRender() = 0;
    virtual void completeFrontBufferRender() = 0;

protected:
    NativeWindow();
    virtual ~NativeWindow() = default;

    bool d_isRoot = false;
    Shared<ApplicationContext> d_applicationContext = nullptr;

    std::function<void()> d_updateCallback;
};
} // namespace mc
