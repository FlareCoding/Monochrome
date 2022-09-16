#include "OSXApplicationContext.h"
#include "OSXNativeWindow.h"

@implementation OSXAppDelegate
@synthesize appContextHandle;

-(void)createDefaultMenu
{
    // Create the menu bar
    NSMenu* menubar = [[NSMenu alloc] init];
    NSMenuItem* menuBarItem = [[NSMenuItem alloc] init];

    // Creating a default 'Quit' menu item
    // with a supported Cmd+Q shortcut handler.
    NSMenu* myMenu = [[NSMenu alloc]init];
    NSString* quitTitle = @"Quit";
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
    action:@selector(terminate:) keyEquivalent:@"q"];
    [myMenu addItem:quitMenuItem];
    [menuBarItem setSubmenu:myMenu];

    // Setting the application's menu bar
    [menubar addItem:menuBarItem];
    [NSApp setMainMenu:menubar];
}

-(void)applicationWillFinishLaunching:(NSNotification*)notification
{
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [self createDefaultMenu];
}

-(void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp stop:nil]; // handing over control to the manual event loop
}

-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    return NSTerminateNow;
}

@end

namespace mc {
    OSXApplicationContext::OSXApplicationContext(const std::string& appId) : ApplicationContext(appId) {
        this->d_appDelegate = [[OSXAppDelegate alloc] init];
        this->d_appDelegate.appContextHandle = this;

        this->d_application = [NSApplication sharedApplication];
        [this->d_application setDelegate:(id)this->d_appDelegate];
    }

    void OSXApplicationContext::startApplicationLoop() {
        // Set the application running flag to true
        d_applicationRunning = true;

        [NSApp run];

        while (d_applicationRunning) {
            _processUiThreadActions();

            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                    untilDate:[NSDate distantFuture]
                                                    inMode:NSDefaultRunLoopMode
                                                    dequeue:YES];

            [NSApp sendEvent:event];

            d_windowLoopMutex.lock();
            for (auto& nativeWindowHandle : d_osxNativeWindowHandles) {
                nativeWindowHandle->updatePlatformWindow();
            }
            d_windowLoopMutex.unlock();
        }
    }

    void OSXApplicationContext::mainWindowRequestedClose() {
        d_applicationRunning = false;
    }

    void OSXApplicationContext::registerOSXNativeWindowHandle(OSXNativeWindow* handle) {
        d_osxNativeWindowHandles.push_back(handle);
    }

    void OSXApplicationContext::unregisterOSXNativeWindowHandle(OSXNativeWindow* handle) {
        for (
            auto it = d_osxNativeWindowHandles.begin();
            it != d_osxNativeWindowHandles.end();
            ++it
        ) {
            if (*it == handle) {
                d_osxNativeWindowHandles.erase(it);
                return;
            }
        }
    }
} // namespace mc
