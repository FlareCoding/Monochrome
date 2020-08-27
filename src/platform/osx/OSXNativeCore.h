#include <Cocoa/Cocoa.h>
#include "OSXWindow.h"

@interface OSXWindowDelegate : NSWindow
@property (nonatomic, assign) mc::OSXWindow* mcWindowHandle;
@end

@interface OSXWindowContentViewDelegate : NSView
@property (nonatomic, assign) mc::OSXWindow* mcWindowHandle;
- (BOOL) isFlipped;
-(void)drawRect:(NSRect)rect;
-(void)update;
@end

struct OSXCocoaNativeObject
{
    NSApplication* application;
    NSWindow* window;
    OSXWindowDelegate* windowDelegate;
    NSView* contentView;
};
