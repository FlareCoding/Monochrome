#import "OSXNativeCore.h"
#include <events/Events.h>

@implementation OSXWindowDelegate
@synthesize mcWindowHandle;

-(BOOL)windowShouldClose:(id)sender
{
    auto e = mc::MakeRef<mc::WindowClosedEvent>(nullptr);
    mcWindowHandle->DispatchEvent(e);
    mcWindowHandle->ProcessImmediateEvents();

    [[NSApplication sharedApplication] stop:nil];
    return YES;
}

-(NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize
{
    auto e = mc::MakeRef<mc::WindowResizedEvent>(nullptr, (uint32_t)frameSize.width, (uint32_t)frameSize.height);
    mcWindowHandle->DispatchEvent(e);
    return frameSize;
}

-(void)windowDidBecomeKey:(NSNotification*)note
{
    auto e = mc::MakeRef<mc::WindowGainedFocusEvent>(nullptr);
    mcWindowHandle->DispatchEvent(e);
}

-(void)windowDidResignKey:(NSNotification*)note
{
    auto e = mc::MakeRef<mc::WindowLostFocusEvent>(nullptr);
    mcWindowHandle->DispatchEvent(e);
}
@end

@implementation OSXWindowContentViewDelegate
@synthesize mcWindowHandle;

- (void) updateTrackingAreas 
{
    const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
                                          NSTrackingActiveInKeyWindow |
                                          NSTrackingEnabledDuringMouseDrag |
                                          NSTrackingMouseMoved |
                                          NSTrackingCursorUpdate |
                                          NSTrackingInVisibleRect |
                                          NSTrackingAssumeInside;

    NSTrackingArea* trackingArea = [[NSTrackingArea alloc] 
                                    initWithRect:[self bounds] 
                                    options: options 
                                    owner:self 
                                    userInfo:nil];

    [self addTrackingArea:trackingArea];
    [super updateTrackingAreas];
}

-(BOOL)isFlipped
{
    return YES;
}

-(BOOL)acceptsFirstResponder
{
    return YES;
}

-(void) drawRect:(NSRect)rect
{
    mcWindowHandle->Update();
}

- (void)update
{
    [self setNeedsDisplay: YES];
}

- (void) mouseDown: (NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };

    auto e = mc::MakeRef<mc::MouseButtonPressedEvent>(click_location, mc::MouseButton::Left);
    mcWindowHandle->DispatchEvent(e);
}

- (void) rightMouseDown: (NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };

    auto e = mc::MakeRef<mc::MouseButtonPressedEvent>(click_location, mc::MouseButton::Right);
    mcWindowHandle->DispatchEvent(e);
}

- (void) mouseUp: (NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };

    auto e = mc::MakeRef<mc::MouseButtonReleasedEvent>(click_location, mc::MouseButton::Left);
    mcWindowHandle->DispatchEvent(e);
}

- (void) rightMouseUp: (NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };

    auto e = mc::MakeRef<mc::MouseButtonReleasedEvent>(click_location, mc::MouseButton::Right);
    mcWindowHandle->DispatchEvent(e);
}

- (void)mouseMoved:(NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;

    NSPoint cursorPoint = [ event locationInWindow ];
    
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };
    mc::Size distance = mc::Size { 0, 0 };
    mc::MouseButton button = mc::MouseButton::None;

    auto mouse_moved_e = mc::MakeRef<mc::MouseMovedEvent>(click_location, distance, button);
    auto hover_on_e = mc::MakeRef<mc::MouseHoverOnEvent>(click_location, distance, button);
    auto hover_off_e = mc::MakeRef<mc::MouseHoverOffEvent>(click_location, distance, button);

    mcWindowHandle->DispatchEvent(mouse_moved_e);
    mcWindowHandle->DispatchEvent(hover_on_e);
    mcWindowHandle->DispatchEvent(hover_off_e);
}

- (void)mouseDragged:(NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;

    NSPoint cursorPoint = [ event locationInWindow ];
    
    mc::Point click_location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };
    mc::Size distance = mc::Size { (float)[event deltaX], (float)[event deltaY] };
    mc::MouseButton button = mc::MouseButton::None;
    NSUInteger buttonMask = [NSEvent pressedMouseButtons];
    if ((buttonMask & (1 << 0)) != 0)
        button = mc::MouseButton::Left;
    else if ((buttonMask & (1 << 1)) != 0)
        button = mc::MouseButton::Right;

    auto mouse_moved_e = mc::MakeRef<mc::MouseMovedEvent>(click_location, distance, button);
    auto hover_on_e = mc::MakeRef<mc::MouseHoverOnEvent>(click_location, distance, button);
    auto hover_off_e = mc::MakeRef<mc::MouseHoverOffEvent>(click_location, distance, button);

    mcWindowHandle->DispatchEvent(mouse_moved_e);
    mcWindowHandle->DispatchEvent(hover_on_e);
    mcWindowHandle->DispatchEvent(hover_off_e);
}

- (void)scrollWheel:(NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;

    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Point location = mc::Point { 
        (float)cursorPoint.x, 
        (float)contentHeight - (float)cursorPoint.y
    };

    float distance = (float)[event deltaY] * 2.0f;

    auto e = mc::MakeRef<mc::MouseScrolledEvent>(location, distance);
    mcWindowHandle->DispatchEvent(e);
}

-(void)keyDown:(NSEvent*) event
{
    NSUInteger flags = [[NSApp currentEvent] modifierFlags];

    bool shift_pressed = (bool)((flags & NSEventModifierFlagShift));
    bool capslock = (bool)((flags & NSEventModifierFlagCapsLock));
    bool repeated = (bool)[[NSApp currentEvent] isARepeat];

    mc::KeyCode keycode = mc::NativeToMcKeycode([event keyCode]);
    auto e = mc::MakeRef<mc::KeyPressedEvent>(keycode, repeated, shift_pressed, capslock);
    mcWindowHandle->DispatchEvent(e);
}

-(void)keyUp:(NSEvent*) event 
{
    mc::KeyCode keycode = mc::NativeToMcKeycode([event keyCode]);
    auto e = mc::MakeRef<mc::KeyReleasedEvent>(keycode);
    mcWindowHandle->DispatchEvent(e);
}
@end
