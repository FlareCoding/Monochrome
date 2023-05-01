#include "LinuxNativeWindow.h"
#include "LinuxApplicationContext.h"
#include <core/InternalFlags.h>
#include <widgets/Layer.h>
#include <events/MouseEvents.h>
#include <events/KeyboardEvents.h>
#include <utils/uuid.h>
#include <utils/Cursor.h>
#include <utils/PlacementConstraintSystem.h>
#include <mutex>

namespace mc {
    static std::mutex s_windowMapMutex;
    static std::vector<LinuxNativeWindow*> s_registeredWindows;

    LinuxNativeWindow::LinuxNativeWindow(uint64_t windowFlags) {
        d_width = 800;
        d_height = 600;
        d_title = "Win32 Window";
        d_windowFlags = windowFlags;

        // Create the render target
        d_renderTarget = MakeRef<LinuxRenderTarget>(d_windowHandle, d_scalingFactor);
    }

    LinuxNativeWindow::~LinuxNativeWindow() {
    }

    void LinuxNativeWindow::setWidth(uint32_t width) {
        d_width = width;
    }

    void LinuxNativeWindow::setHeight(uint32_t height) {
        d_height = height;
    }

    void LinuxNativeWindow::setMinWidth(uint32_t minWidth) {
        d_minSize.width = minWidth;
    }

    void LinuxNativeWindow::setMaxWidth(uint32_t maxWidth) {
        d_maxSize.width = maxWidth;
    }

    void LinuxNativeWindow::setMinHeight(uint32_t minHeight) {
        d_minSize.height = minHeight;
    }

    void LinuxNativeWindow::setMaxHeight(uint32_t maxHeight) {
        d_maxSize.height = maxHeight;
    }

    void LinuxNativeWindow::setPosition(const Position& pos) {
        d_position = pos;
    }

    void LinuxNativeWindow::setTitle(const std::string& title) {
        d_title = title;
    }

    void LinuxNativeWindow::show() {
        CORE_ASSERT(!d_visible, "Window is already opened, you called 'show()' twice");

        d_visible = true;
    }

    void LinuxNativeWindow::hide() {
        CORE_ASSERT(d_visible, "Window is already hidden, you called 'hide()' twice");

        d_visible = false;
    }

    void LinuxNativeWindow::focus() {
    }

    void LinuxNativeWindow::unfocus() {
    }

    void LinuxNativeWindow::close() {
    }

    void LinuxNativeWindow::maximize() {
    }

    void LinuxNativeWindow::restoreMaximize() {
    }

    void LinuxNativeWindow::minimize() {
    }

    void LinuxNativeWindow::requestRedraw() {
    }

} // namespace mc
