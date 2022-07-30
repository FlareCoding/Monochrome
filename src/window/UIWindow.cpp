#include "UIWindow.h"
#include <rendering/Renderer.h>
#include <widgets/FlowPanel.h>
#include <chrono>

#ifdef MC_PLATFORM_MACOS
#include <platform/macos/OSXNativeWindow.h>
#endif

namespace mc {
    // Specifies that only the first created window should be the root window
    static bool s_isFirstWindow = true;

    UIWindow::UIWindow(
        uint64_t windowFlags,
        uint32_t width,
        uint32_t height,
        const std::string& title
    ) {
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

            // If the body flowpanel is present, stretch
            // the body panel to fit into the window.
            if (d_bodyPanel) {
                adjustBodyPanel();
            }

            setShouldRedraw();
        });

        on("focusChanged", [this](Shared<Event> event) {
            auto focused = event->get<bool>("focused");

            if (focused) {
                setShouldRedraw();
            }
        });

        // For high-frequency events such as mouseMoved, the window
        // should swap buffers immediately after rendering to produce
        // a smooth update of display and prevent the visual lag.
        on("mouseMoved", [this](Shared<Event> e) {
            _requestOnDemandBufferSwap();
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

    Size UIWindow::getSize() {
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
        if (!shouldRedraw()) {
            d_nativeWindow->getRenderTarget()->swapBuffers();
        }

#ifdef MC_PLATFORM_MACOS
        // Due to the way Cocoa rendering system doesn't let you
        // reliably control the timing of drawRect being called
        // preventing you from properly synchronizing buffer swapping,
        // while resizing, the scene has to be re-rendered manually
        // to the front buffer directly.
        auto osxWindow = std::static_pointer_cast<OSXNativeWindow>(d_nativeWindow);

        if (osxWindow->isFrontBufferRenderRequested()) {
            auto renderTarget = d_nativeWindow->getRenderTarget();
            renderTarget->lockBackBuffer();
            renderTarget->beginFrame();

            _renderScene(renderTarget);

            renderTarget->endFrame();
            renderTarget->unlockBackBuffer();
            renderTarget->swapBuffers();

            // Complete the request
            osxWindow->completeFrontBufferRender();
        }
#endif
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

    Shared<FlowPanel> UIWindow::getBody() {
        if (!d_bodyPanel) {
            d_bodyPanel = MakeRef<FlowPanel>();
            d_bodyPanel->layout = Horizontal;
            d_bodyPanel->backgroundColor = Color::transparent;
            adjustBodyPanel();
            addWidget(d_bodyPanel);
        }

        return d_bodyPanel;
    }

    void UIWindow::setBodyPanelOffset(const Size& offset) {
        d_bodyPanelOffset = offset;
        if (d_bodyPanel) {
            adjustBodyPanel();
        }
    }

    void UIWindow::adjustBodyPanel() {
        d_bodyPanel->position = {
            static_cast<int32_t>(d_bodyPanelOffset.width),
            static_cast<int32_t>(d_bodyPanelOffset.height)
        };
        d_bodyPanel->size = {
            static_cast<uint32_t>(getWidth() - d_bodyPanelOffset.width),
            static_cast<uint32_t>(getHeight() - d_bodyPanelOffset.height)
        };
    }

    void UIWindow::_backgroundRenderingTask() {
        while (!d_isDestroyed) {
#ifdef MC_PLATFORM_MACOS
            // If the current platform is MacOS, to prevent screen
            // flickering, the background thread has to be paused
            // while the window is resizing.
            auto osxWindow = std::static_pointer_cast<OSXNativeWindow>(d_nativeWindow);
            if (osxWindow->isFrontBufferRenderRequested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }
#endif

            if (shouldRedraw()) {
                auto renderTarget = d_nativeWindow->getRenderTarget();
                renderTarget->lockBackBuffer();
                renderTarget->beginFrame();

                _renderScene(renderTarget);

                renderTarget->endFrame();
                renderTarget->unlockBackBuffer();

                if (_shouldSwapBuffersOnDemand()) {
                    renderTarget->swapBuffers();
                    _completeOnDemandBufferSwap();
                }

                // Mark the scene as successfully drawn
                d_shouldRedrawScene = false;

                // Request the native window to
                // repaint with the updated contents.
                d_nativeWindow->requestRedraw();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void UIWindow::_renderScene(Shared<RenderTarget>& renderTarget) {
        Renderer::renderScene(d_backgroundColor, *d_widgetHostController, renderTarget);
    }
} // namespace mc
