#include "NativeWindow.h"

#ifdef MC_PLATFORM_WINDOWS
    #include <platform/windows/Win32NativeWindow.h>
#elif defined(MC_PLATFORM_MACOS)
    #include <platform/macos/OSXNativeWindow.h>
#endif

namespace mc {
    NativeWindow::NativeWindow() {
        d_applicationContext = AppManager::getApplicationContext();

        this->appendAllowedEvent("sizeChanged");
        this->appendAllowedEvent("focusChanged");
        this->appendAllowedEvent("mouseMoved");
        this->appendAllowedEvent("mouseDown");
        this->appendAllowedEvent("mouseUp");
        this->appendAllowedEvent("keyDown");
        this->appendAllowedEvent("keyUp");

        // Special types of events called
        // from low level system hooks.
        this->appendAllowedEvent("globalMouseDown");
        this->appendAllowedEvent("mouseEnteredWindow");
        this->appendAllowedEvent("mouseLeftWindow");
    }

    Shared<NativeWindow> NativeWindow::create(bool isRoot, uint64_t windowFlags) {
        Shared<NativeWindow> instance = nullptr;

#ifdef MC_PLATFORM_WINDOWS
        instance = Shared<NativeWindow>(new Win32NativeWindow(windowFlags));
#elif defined(MC_PLATFORM_MACOS)
        instance = Shared<NativeWindow>(new OSXNativeWindow(windowFlags));
#endif

        CORE_ASSERT(instance, "Failed to create native window, unknown platform");

        instance->d_isRoot = isRoot;
        return instance;
    }
} // namespace mc
