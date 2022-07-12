#include "UIWindow.h"
#include <rendering/Renderer.h>
#include <chrono>

namespace mc
{
    // Specifies that only the first created window should be the root window
    static bool s_isFirstWindow = true;

    UIWindow::UIWindow(uint64_t windowFlags, uint32_t width, uint32_t height, const std::string& title) {
        d_uuid = utils::generateUUID();

        // Create the native window
        d_nativeWindow = NativeWindow::create(s_isFirstWindow, windowFlags);
        d_nativeWindow->forwardEmittedEvents(this);
        d_nativeWindow->setWidth(width);
        d_nativeWindow->setHeight(height);
        d_nativeWindow->setTitle(title);
        d_nativeWindow->setUpdateCallback([this]() { this->update(); });

        d_nativeWindow->show();

        if (s_isFirstWindow) {
            s_isFirstWindow = false;
        }

        // Setup event handlers
        on("sizeChanged", [this](Shared<Event> event) {
            auto width = event->get<uint32_t>("width");
            auto height = event->get<uint32_t>("height");

            setShouldRedraw();
        });

        on("focusChanged", [this](Shared<Event> event) {
            auto focused = event->get<bool>("focused");

            if (focused) {
                setShouldRedraw();
            }
        });

        // Setup the background rendering thread
        d_renderingThread = std::thread(&UIWindow::_backgroundRenderingTask, this);

        // Setup widget host controller
        d_widgetHostController = MakeRef<WidgetHostController>();
        d_widgetHostController->on("widgetTreeChanged", [this](Shared<Event> e) {
            setShouldRedraw();
        });
        this->forwardEmittedEvents(d_widgetHostController.get());

        // Setup the event handling callbacks
        d_widgetHostController->on("mouseMoved", [this](auto e) {
            d_widgetHostController->processMouseMovedEvent(
                std::static_pointer_cast<MouseMovedEvent>(e)
            );
        });

        d_widgetHostController->on("mouseDown", [this](auto e) {
            d_widgetHostController->processMousePressedEvent(
                std::static_pointer_cast<MouseButtonEvent>(e)
            );
        });

        d_widgetHostController->on("mouseUp", [this](auto e) {
            d_widgetHostController->processMouseReleasedEvent(
                std::static_pointer_cast<MouseButtonEvent>(e)
            );
        });

        d_widgetHostController->on("keyDown", [this](auto e) {
            d_widgetHostController->processKeyDownEvent(
                std::static_pointer_cast<KeyDownEvent>(e)
            );
        });

        d_widgetHostController->on("keyUp", [this](auto e) {
            d_widgetHostController->processKeyUpEvent(
                std::static_pointer_cast<KeyUpEvent>(e)
            );
        });
    }

    UIWindow::~UIWindow() {
        d_isDestroyed = true;
        d_renderingThread.join();
    }

    uint32_t UIWindow::getWidth() {
        return d_nativeWindow->getWidth();
    }

    uint32_t UIWindow::getHeight() {
        return d_nativeWindow->getHeight();
    }

    const std::string& UIWindow::getTitle() {
        return d_nativeWindow->getTitle();
    }

    Position UIWindow::getPosition() {
        return d_nativeWindow->getPosition();
    }

    Size UIWindow::getSize()
    {
        return d_nativeWindow->getSize();
    }

    void UIWindow::setWidth(uint32_t width) {
        d_nativeWindow->setWidth(width);
    }

    void UIWindow::setHeight(uint32_t height) {
        d_nativeWindow->setHeight(height);
    }

    void UIWindow::setSize(uint32_t width, uint32_t height) {
        d_nativeWindow->setWidth(width);
        d_nativeWindow->setHeight(height);
    }

    void UIWindow::setPosition(const Position& pos) {
        d_nativeWindow->setPosition(pos);
    }

    void UIWindow::setTitle(const std::string& title) {
        d_nativeWindow->setTitle(title);
    }

    bool UIWindow::isFocused() const {
        return d_nativeWindow->isFocused();
    }

    void UIWindow::show() {
        d_nativeWindow->show();
    }

    void UIWindow::hide() {
        d_nativeWindow->hide();
    }

    void UIWindow::focus() {
        d_nativeWindow->focus();
    }

    void UIWindow::unfocus() {
        d_nativeWindow->unfocus();
    }

    void UIWindow::close() {
        d_nativeWindow->close();
    }

    void UIWindow::maximize() {
        d_nativeWindow->maximize();
    }

    void UIWindow::restoreMaximize() {
        d_nativeWindow->restoreMaximize();
    }
    
    void UIWindow::minimize() {
        d_nativeWindow->minimize();
    }

    void UIWindow::update() {
        if (d_renderTargetBuffersReadyForSwap) {
            d_nativeWindow->getRenderTarget()->swapBuffers();

            d_renderTargetBuffersReadyForSwap = false;
        }
    }

    void UIWindow::setShouldRedraw() {
        d_shouldRedrawScene = true;
    }

    void UIWindow::addWidget(Shared<BaseWidget> widget) {
        d_widgetHostController->addWidget(widget);
    }

    bool UIWindow::removeWidget(Shared<BaseWidget> widget) {
        return d_widgetHostController->removeWidget(widget);
    }

    bool UIWindow::removeWidget(uuid_t uuid) {
        return d_widgetHostController->removeWidget(uuid);
    }

    void UIWindow::removeAllWidgets() {
        d_widgetHostController->removeAllWidgets();
    }

    Shared<BaseWidget> UIWindow::findWidget(uuid_t uuid) {        
        return d_widgetHostController->findWidget(uuid);
    }

    void UIWindow::_backgroundRenderingTask() {
        while (!d_isDestroyed) {
            if (shouldRedraw()) {
                auto renderTarget = d_nativeWindow->getRenderTarget();
                renderTarget->lockBackBuffer();
                renderTarget->beginFrame();

                _renderScene(renderTarget);

                renderTarget->endFrame();
                renderTarget->unlockBackBuffer();

                d_renderTargetBuffersReadyForSwap = true;
                d_shouldRedrawScene = false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void UIWindow::_renderScene(Shared<RenderTarget>& renderTarget) {
        Renderer::renderScene(d_backgroundColor, *d_widgetHostController, renderTarget);
    }
}
