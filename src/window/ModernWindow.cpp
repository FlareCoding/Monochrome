#include "ModernWindow.h"
#include <core/InternalFlags.h>

#define MC_MODERNWINDOW_FLAGS  (1ULL << WindowStyleBorderless) |  \
                                (1ULL << WindowStyleResizable) |  \
                                (1ULL << WindowStyleDraggable) |  \
                                (1ULL << WindowStyleMinimizable)

namespace mc {
    ModernWindow::ModernWindow(uint32_t width, uint32_t height, const std::string& title)
        : UIWindow(MC_MODERNWINDOW_FLAGS, width, height, title) {
        // Create the 'close', 'minimize', and 'maximize'
        // buttons as well as the title label.
        // _createDecoratorWidgets();

        // Setup events handlers
        // _setupEventHandlers();

        // Adjust the offset of the body panel that belongs to a UIWindow
        //setBodyPanelOffset({ 0, d_titleLabel->size->height + 8 });
    }

    void ModernWindow::setBackgroundColor(const Color& color) {
        UIWindow::setBackgroundColor(color);
    }

    void ModernWindow::setTitle(const std::string& title) {
        UIWindow::setTitle(title);
    }

    void ModernWindow::_createDecoratorWidgets() {
    }

    void ModernWindow::_setupEventHandlers() {
    }
} // namespace mc
