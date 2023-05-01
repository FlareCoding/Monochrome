#pragma once
#include <application/ApplicationContext.h>
#include <vector>
#include <atomic>

namespace mc {
class LinuxNativeWindow;

class LinuxApplicationContext : public ApplicationContext {
public:
    explicit LinuxApplicationContext(const std::string& appId);

    void startApplicationLoop();

    void mainWindowRequestedClose();
    void registerLinuxNativeWindowHandle(LinuxNativeWindow* handle);

private:
    std::vector<LinuxNativeWindow*> d_linuxNativeWindowHandles;
    std::atomic_bool d_applicationRunning = false;
};
} // namespace mc
