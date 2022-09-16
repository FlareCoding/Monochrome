#pragma once
#include <application/ApplicationContext.h>
#include <Cocoa/Cocoa.h>
#include <atomic>
#include <vector>
#include <mutex>

@interface OSXAppDelegate : NSObject
@property (nonatomic, assign) void* appContextHandle;
@end

namespace mc {
class OSXNativeWindow;

class OSXApplicationContext : public ApplicationContext {
public:
    explicit OSXApplicationContext(const std::string& appId);

    void startApplicationLoop();
    void mainWindowRequestedClose();
    void registerOSXNativeWindowHandle(OSXNativeWindow* handle);
    void unregisterOSXNativeWindowHandle(OSXNativeWindow* handle);

private:
    std::vector<OSXNativeWindow*> d_osxNativeWindowHandles;
    std::mutex d_windowLoopMutex;

    NSApplication*      d_application = nullptr;
    OSXAppDelegate*     d_appDelegate = nullptr;
    std::atomic_bool    d_applicationRunning = false;
};
} // namespace mc
