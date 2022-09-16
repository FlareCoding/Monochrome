#include "ApplicationContext.h"

#ifdef MC_PLATFORM_WINDOWS_X64
    #include <platform/windows/Win32ApplicationContext.h>
#elif defined(MC_PLATFORM_MACOS)
    #include <platform/macos/OSXApplicationContext.h>
#endif

namespace mc {
    Shared<ApplicationContext> ApplicationContext::create(const std::string& appId) {
#ifdef MC_PLATFORM_WINDOWS_X64
        return Shared<Win32ApplicationContext>(new Win32ApplicationContext(appId));
#elif defined(MC_PLATFORM_MACOS)
        return Shared<OSXApplicationContext>(new OSXApplicationContext(appId));
#endif
    }

    void ApplicationContext::dispatchActionToUiThread(std::function<void()> fn) {
        d_uiThreadActionQueue.push_back(fn);
    }

    ApplicationContext::ApplicationContext(const std::string& appId) : d_appId(appId) {}

    void ApplicationContext::_processUiThreadActions() {
        // First check if there are any actions that need processing
        if (d_uiThreadActionQueue.empty()) {
            return;
        }

        // Lock the thread mutex
        d_uiThreadMutex.lock();

        // Call all the callback functions from the UI thread
        for (auto& fn : d_uiThreadActionQueue) {
            fn();
        }

        // Clear the actions queue
        d_uiThreadActionQueue.clear();

        // Unlock the mutex
        d_uiThreadMutex.unlock();
    }
} // namespace mc
