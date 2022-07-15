#pragma once
#include <application/ApplicationContext.h>
#include <Cocoa/Cocoa.h>

@interface OSXAppDelegate : NSObject
@property (nonatomic, assign) void* appContextHandle;
@end

namespace mc {
class OSXApplicationContext : public ApplicationContext {
public:
    explicit OSXApplicationContext(const std::string& appId);

    void startApplicationLoop();

private:
    NSApplication*  d_application = nullptr;
    OSXAppDelegate* d_appDelegate = nullptr;
};
} // namespace mc
