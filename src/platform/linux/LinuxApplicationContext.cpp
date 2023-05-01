#include "LinuxApplicationContext.h"
#include "LinuxNativeWindow.h"
#include <thread>

namespace mc {
    LinuxApplicationContext::LinuxApplicationContext(const std::string& appId)
        : ApplicationContext(appId) {}

    void LinuxApplicationContext::mainWindowRequestedClose() {
        d_applicationRunning = false;
    }

    void LinuxApplicationContext::registerLinuxNativeWindowHandle(LinuxNativeWindow* handle) {
        d_linuxNativeWindowHandles.push_back(handle);
    }

    void LinuxApplicationContext::startApplicationLoop() {
        // Check if any of the window's are opened
        if (!d_linuxNativeWindowHandles.size()) {
            return;
        }


        // Set the application running flag to true
        d_applicationRunning = true;

        // Start the application loop
        while (d_applicationRunning) {
            break;
        }

        // Destroy all windows
        for (auto& window : d_linuxNativeWindowHandles) {
        }
    }
} // namespace mc
