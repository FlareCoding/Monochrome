#include "OSXApplicationContext.h"

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
}

-(NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)sender
{
    return NSTerminateNow;
}

@end

namespace mc
{
    OSXApplicationContext::OSXApplicationContext(const std::string& appId) : ApplicationContext(appId) {
        this->d_appDelegate = [[OSXAppDelegate alloc] init];
        this->d_appDelegate.appContextHandle = this;

        this->d_application = [NSApplication sharedApplication];
        [this->d_application setDelegate:(id)this->d_appDelegate];
    }

    void OSXApplicationContext::startApplicationLoop() {
        [NSApp run];
    }
}
