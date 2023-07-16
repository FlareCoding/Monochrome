#include "UIWindow.h"
#include <rendering/Renderer.h>
#include <chrono>
#include <widgets/BaseWidget.h>
#include <widgets/StackPanel.h>

#ifdef MC_PLATFORM_MACOS
#include <platform/macos/OSXNativeWindow.h>
#endif

#ifdef MC_ENABLE_PERF_PROFILING
#include <core/FunctionProfiler.h>
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

        d_nativeWindow->show();

        if (s_isFirstWindow) {
            s_isFirstWindow = false;
        }

        // Setup event handlers
        on("sizeChanged", [this](Shared<Event> event) {
            auto width = event->get<uint32_t>("width");
            auto height = event->get<uint32_t>("height");

            if (d_rootWidget) {
                d_rootWidget->setComputedSize(getSize());
                d_rootWidget->fireEvent("layoutChanged", Event::empty);
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

        // Create the event processor
        d_eventProcessor = MakeRef<EventProcessor>();
        d_eventProcessor->on("widgetTreeChanged", [this](Shared<Event> e) {
            setShouldRedraw();
        });

        on("mouseDown",     &EventProcessor::processMouseDownEvent,         d_eventProcessor.get());
        on("mouseUp",       &EventProcessor::processMouseUpEvent,           d_eventProcessor.get());
        on("mouseMoved",    &EventProcessor::processMouseMovedEvent,        d_eventProcessor.get());
        on("mouseScrolled", &EventProcessor::processMouseScrolledEvent,     d_eventProcessor.get());
        on("keyDown",       &EventProcessor::processKeyDownEvent,           d_eventProcessor.get());
        on("keyUp",         &EventProcessor::processKeyUpEvent,             d_eventProcessor.get());

        // Setup the background rendering thread
        d_renderingThread = std::thread(&UIWindow::_backgroundRenderingTask, this);

        // Pass the update callback to the native window
        d_nativeWindow->setUpdateCallback([this]() { this->update(); });
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

    void UIWindow::setMinWidth(uint32_t minWidth) {
        d_nativeWindow->setMinWidth(minWidth);
    }

    void UIWindow::setMaxWidth(uint32_t maxWidth) {
        d_nativeWindow->setMaxWidth(maxWidth);
    }

    void UIWindow::setMinHeight(uint32_t minHeight) {
        d_nativeWindow->setMinHeight(minHeight);
    }

    void UIWindow::setMaxHeight(uint32_t maxHeight) {
        d_nativeWindow->setMaxHeight(maxHeight);
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
        // Due to the way Cocoa and Direct2D rendering systems don't let you
        // reliably control the timing of content rendering being called
        // preventing you from properly synchronizing buffer swapping,
        // while resizing, the scene has to be re-rendered manually
        // to the front buffer directly.
        if (d_nativeWindow->isFrontBufferRenderRequested()) {
            // When an update happens due to a window manipulation event
            // such as resizing, the background rendering thread and the
            // forced render here should be synchronized.
            d_renderingSyncMutex.lock();

            auto renderTarget = d_nativeWindow->getRenderTarget();

            // When the window is being resized, due to the constantly changing size
            // of the render target, it's better to not draw the canvas overlay.
            _processRenderingCycle(renderTarget, false);

            // Release the synchornization lock
            d_renderingSyncMutex.unlock();

            // Update the displayed frame
            renderTarget->swapBuffers();

            // Complete the request
            d_nativeWindow->completeFrontBufferRender();
        } else if (!shouldRedraw()) {
            d_nativeWindow->getRenderTarget()->swapBuffers();
        }
    }

    void UIWindow::setShouldRedraw() {
        d_shouldRedrawScene = true;
    }

    void UIWindow::setRootWidget(Shared<BaseContainerWidget> root) {
        d_eventProcessor->handlePotentialFocusChanged(nullptr);

        if (d_rootWidget) {
            d_rootWidget->off("propertyChanged");
            d_rootWidget->off("layoutChanged");
            d_rootWidget->off("requestedFocusGain");
            d_rootWidget->off("requestedFocusLoss");
        }

        d_rootWidget = root;
        d_rootWidget->setComputedSize(getSize());
        d_rootWidget->on("layoutChanged", [this](Shared<Event> e) {
            // Measure phase will measure all the child widgets
            // recursively and set each child's desired size.
            d_rootWidget->measure();

            // First we need to set the current widget's computed size
            d_rootWidget->setComputedSize(getSize());

            // After the computed size has been set,
            // recursively arrange all the children inside.
            d_rootWidget->arrangeChildren();
        });

        d_rootWidget->on("propertyChanged", [this](Shared<Event> e) {
            // Since the widget tree has changed, a redraw is required
            setShouldRedraw();
        });

        d_rootWidget->on("requestedFocusGain", [this](Shared<Event> e) {
            BaseWidget* target = e->get<BaseWidget*>("target");
            d_eventProcessor->handlePotentialFocusChanged(target);
        });

        d_rootWidget->on("requestedFocusLoss", [this](Shared<Event> e) {
            d_eventProcessor->handlePotentialFocusChanged(nullptr);
        });

        setShouldRedraw();

        // Update the event processor's root widget
        d_eventProcessor->setRootWidget(d_rootWidget);

        // Fire the layout changed event to trigger a first time root layout update
        d_rootWidget->fireEvent("layoutChanged", Event::empty);
    }

    Shared<Canvas2D> UIWindow::createOverlayCanvas() {
        d_overlayCanvas = MakeRef<Canvas2D>();

        auto renderTarget = d_nativeWindow->getRenderTarget();
        d_overlayCanvas->setRenderTarget(renderTarget);

        return d_overlayCanvas;
    }

    void UIWindow::_backgroundRenderingTask() {
        while (!d_isDestroyed) {
            // To prevent screen from flickering, the background
            // thread has to be paused while the window is resizing.
            if (d_nativeWindow->isFrontBufferRenderRequested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
                continue;
            }

            if (shouldRedraw()) {
                // When an update happens due to a window manipulation event
                // such as resizing, the background rendering thread and the
                // window update thread should get synchronized.
                d_renderingSyncMutex.lock();

                auto renderTarget = d_nativeWindow->getRenderTarget();
                _processRenderingCycle(renderTarget);

                // Release the synchronization lock
                d_renderingSyncMutex.unlock();

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
#ifdef MC_ENABLE_PERF_PROFILING
        PROFILE_FUNCTION("renderScene");
#endif

        Renderer::renderScene(d_backgroundColor, d_rootWidget, renderTarget);

#ifdef MC_ENABLE_PERF_PROFILING_OVERLAY
        profiler_309.endProfiling();

        _displayProfilingOverlay(renderTarget);
#endif
    }

    void UIWindow::_processRenderingCycle(
        Shared<RenderTarget>& renderTarget,
        bool renderOverlayCanvas
    ) {
        // Prepare the back buffer for rendering
        renderTarget->lockBackBuffer();

        // The scene should first get rendered onto the offscreen bitmap
        renderTarget->beginOffscreenSceneFrame();

        // Render the scene to the offscreen render target
        _renderScene(renderTarget);

        // End the frame and capture the rendered scene bitmap
        renderTarget->endOffscreenSceneFrame();

        // Begin the actual frame on the main render target
        renderTarget->beginFrame();

        // First draw the rendered scene bitmap
        renderTarget->drawOffscreenSceneBitmap();

        // Then if necessary, render the overlay canvas elements
        if (renderOverlayCanvas && d_overlayCanvas) {
            d_overlayCanvas->__render();
        }

        // End the main render target frame and
        // commit the rendered data to the main window.
        renderTarget->endFrame();

        // Mark the end of the rendering cycle
        renderTarget->unlockBackBuffer();
    }

#ifdef MC_ENABLE_PERF_PROFILING_OVERLAY
    void UIWindow::_displayProfilingOverlay(Shared<RenderTarget>& renderTarget) {
        // Create a semi-transparent overlay region
        renderTarget->drawRectangle(0, 0, 400, 140, Color(20, 20, 20, 140), 0, true, 0);

        _drawFunctionPerfProfileRecord(renderTarget, "renderScene", 20, 20);
        _drawFunctionPerfProfileRecord(renderTarget, "processMouseDownEvent", 20, 40);
        _drawFunctionPerfProfileRecord(renderTarget, "processMouseUpEvent", 20, 60);
        _drawFunctionPerfProfileRecord(renderTarget, "processMouseMovedEvent", 20, 80);
    }

    void UIWindow::_drawFunctionPerfProfileRecord(
        Shared<RenderTarget>& renderTarget,
        const std::string& fnName,
        int32_t xPos,
        int32_t yPos
    ) {
        // Get the function profiler record
        auto& perfProfileRegistry = mc::debug::GlobalFunctionProfilerRegistry::get();
        auto& record = perfProfileRegistry.getProfilerSessionRecord(fnName);

        // Convert the time from microseconds to seconds
        double timeInSeconds =
            static_cast<double>(record.durationInMicroseconds) / 1000 / 1000;

        std::string renderingDurationStr = std::to_string(timeInSeconds);

        // Render the text
        std::string perfStr = fnName + ": " + renderingDurationStr + "s";

        renderTarget->drawText(
            xPos, yPos,
            400, 20,
            Color(255, 255, 0),
            perfStr,
            "Arial", 16, "normal", "left"
        );
    }
#endif
} // namespace mc
