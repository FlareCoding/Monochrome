#include "ClassicWindow.h"
#include <core/InternalFlags.h>

#define MC_CLASSICWINDOW_FLAGS  (1ULL << WindowStyleClosable)    |  \
                                (1ULL << WindowStyleMinimizable) |  \
                                (1ULL << WindowStyleMaximizable) |  \
                                (1ULL << WindowStyleTitled)      |  \
                                (1ULL << WindowStyleResizable)

namespace mc
{
    ClassicWindow::ClassicWindow(uint32_t width, uint32_t height, const std::string& title)
        : UIWindow(MC_CLASSICWINDOW_FLAGS, width, height, title) {}
}
