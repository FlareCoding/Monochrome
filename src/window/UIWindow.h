#pragma once
#include "NativeWindow.h"
#include <widgets/WidgetHostController.h>
#include <utils/uuid.h>
#include <thread>
#include <atomic>

namespace mc
{
    class UIWindow : public EventEmitter
    {
    public:
        UIWindow(uint64_t windowFlags, uint32_t width = 800, uint32_t height = 600, const std::string& title = "Window");
        virtual ~UIWindow();

        uint32_t getWidth();
        uint32_t getHeight();
        const std::string& getTitle();
        Position getPosition();
        void setWidth(uint32_t width);
        void setHeight(uint32_t height);
        void setSize(uint32_t width, uint32_t height);
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

        void addWidget(Shared<BaseWidget> widget);
        bool removeWidget(Shared<BaseWidget> widget);
        bool removeWidget(uuid_t uuid);
        void removeAllWidgets();
        Shared<BaseWidget> findWidget(uuid_t uuid);

        Color getBackgroundColor() const { return d_backgroundColor; }
        virtual void setBackgroundColor(const Color& color) { d_backgroundColor = color; }

    private:
        Shared<NativeWindow> d_nativeWindow = nullptr;
        Shared<WidgetHostController> d_widgetHostController = nullptr;

        uuid_t d_uuid = 0;
        std::atomic_bool d_isDestroyed = false;
        Color d_backgroundColor = Color::darkGray;

        std::thread d_renderingThread;

        std::atomic_bool d_shouldRedrawScene = true;
        std::atomic_bool d_renderTargetBuffersReadyForSwap = false;

        void _backgroundRenderingTask();
        void _renderScene(Shared<RenderTarget>& renderTarget);
    };
}
