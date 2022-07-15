#include "OSXNativeWindow.h"
#include "OSXApplicationContext.h"
#include <core/InternalFlags.h>
#include <widgets/Layer.h>
#include <events/MouseEvents.h>
#include <events/KeyboardEvents.h>
#include <utils/PlacementConstraintSystem.h>
using namespace mc;

namespace mc {
    static std::mutex s_windowMapMutex;
	static std::vector<OSXNativeWindow*> s_registeredWindows;

	static void registerNativeWindow(OSXNativeWindow* window) {
		s_windowMapMutex.lock();
		s_registeredWindows.push_back(window);
		s_windowMapMutex.unlock();
	}

	static void unregisterNativeWindow(OSXNativeWindow* window) {
		s_windowMapMutex.lock();

		s_registeredWindows.erase(
			std::remove(
				s_registeredWindows.begin(),
				s_registeredWindows.end(),
				window
			)
		);

		s_windowMapMutex.unlock();
	}
}

@implementation OSXWindowDelegate
@synthesize mcWindowHandle;

-(BOOL)windowShouldClose:(NSWindow*)sender
{
    OSXWindowDelegate* delegate = ((OSXWindowDelegate*)sender.delegate);
    OSXNativeWindow* windowInstance = delegate.mcWindowHandle;

    // Unregister the window instance
    unregisterNativeWindow(windowInstance);

    // Stop the application only if the
    // destroyed window was the root window.
    if (windowInstance->isRoot()) {
        [[NSApplication sharedApplication] stop:nil];
    }

    return YES;
}

-(NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize
{
    auto resizeEvent = MakeRef<Event>(eventDataMap_t{
        { "width", uint32_t(frameSize.width) },
        { "height", uint32_t(frameSize.height) }
    });

    OSXWindowDelegate* delegate = ((OSXWindowDelegate*)sender.delegate);
    OSXNativeWindow* windowInstance = delegate.mcWindowHandle;

    windowInstance->fireEvent("sizeChanged", resizeEvent);
    return frameSize;
}

-(void)windowDidBecomeKey:(NSNotification*)notification
{
    OSXNativeWindow* windowInstance = self.mcWindowHandle;

    auto focusChangedEvent = MakeRef<Event>(eventDataMap_t{
        { "focused", true }
    });

    windowInstance->fireEvent("focusChanged", focusChangedEvent);
}

-(void)windowDidResignKey:(NSNotification*)notification
{
    OSXNativeWindow* windowInstance = self.mcWindowHandle;

    auto focusChangedEvent = MakeRef<Event>(eventDataMap_t{
        { "focused", false }
    });

    windowInstance->fireEvent("focusChanged", focusChangedEvent);
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
    return NO;
}

-(BOOL)acceptsFirstResponder
{
    return YES;
}

-(void) drawRect:(NSRect)rect
{
    Shared<RenderTarget> renderTarget = self.mcWindowHandle->getRenderTarget();
    NSImage* frontBuffer = reinterpret_cast<NSImage*>(renderTarget->getFrontBuffer());

    [frontBuffer    drawAtPoint:NSMakePoint(0,0)
                    fromRect:CGRectMake(
                        0, 0,
                        frontBuffer.size.width,
                        frontBuffer.size.height
                    )
                    operation:NSCompositingOperationCopy fraction:1.0];
}

- (void)update
{
    auto updateCallback = self.mcWindowHandle->getUpdateCallback();
    
    if (updateCallback) {
        updateCallback();
    }

    [self setNeedsDisplay: YES];
}

- (void) mouseDown: (NSEvent*) event
{
    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    if (!windowInstance->isFocused()) {
        return;
    }

    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];

    NSPoint screenCursorPoint = [window convertPointToScreen:cursorPoint];
    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

    mc::Position screenLocation = mc::Position(
        (int32_t)screenCursorPoint.x,
        (int32_t)(screenHeight - screenCursorPoint.y)
    );

    auto mouseDownEvent = MakeRef<Event>(eventDataMap_t{
        { "location", mc::Position((uint32_t)cursorPoint.x, (uint32_t)(contentHeight - cursorPoint.y)) },
        { "screenLocation", screenLocation },
        { "button", MouseButton::Left }
    });

    windowInstance->fireEvent("mouseDown", mouseDownEvent);
}

- (void) rightMouseDown: (NSEvent*) event
{
    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    if (!windowInstance->isFocused()) {
        return;
    }

    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];

    NSPoint screenCursorPoint = [window convertPointToScreen:cursorPoint];
    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

    mc::Position screenLocation = mc::Position(
        (int32_t)screenCursorPoint.x,
        (int32_t)(screenHeight - screenCursorPoint.y)
    );

    auto mouseDownEvent = MakeRef<Event>(eventDataMap_t{
        { "location", mc::Position((uint32_t)cursorPoint.x, (uint32_t)(contentHeight - cursorPoint.y)) },
        { "screenLocation", screenLocation },
        { "button", MouseButton::Right }
    });

    windowInstance->fireEvent("mouseDown", mouseDownEvent);
}

- (void) mouseUp: (NSEvent*) event
{
    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    if (!windowInstance->isFocused()) {
        return;
    }

    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];
    
    NSPoint screenCursorPoint = [window convertPointToScreen:cursorPoint];
    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

    mc::Position screenLocation = mc::Position(
        (int32_t)screenCursorPoint.x,
        (int32_t)(screenHeight - screenCursorPoint.y)
    );

    auto mouseUpEvent = MakeRef<Event>(eventDataMap_t{
        { "location", mc::Position((int32_t)cursorPoint.x, (int32_t)(contentHeight - cursorPoint.y)) },
        { "screenLocation", screenLocation },
        { "button", MouseButton::Left }
    });

    windowInstance->fireEvent("mouseUp", mouseUpEvent);
}

- (void) rightMouseUp: (NSEvent*) event
{
    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    if (!windowInstance->isFocused()) {
        return;
    }

    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;
    NSPoint cursorPoint = [ event locationInWindow ];

    NSPoint screenCursorPoint = [window convertPointToScreen:cursorPoint];
    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

    mc::Position screenLocation = mc::Position(
        (int32_t)screenCursorPoint.x,
        (int32_t)(screenHeight - screenCursorPoint.y)
    );

    auto mouseUpEvent = MakeRef<Event>(eventDataMap_t{
        { "location", mc::Position((uint32_t)cursorPoint.x, (uint32_t)(contentHeight - cursorPoint.y)) },
        { "screenLocation", screenLocation },
        { "button", MouseButton::Right }
    });

    windowInstance->fireEvent("mouseUp", mouseUpEvent);
}

- (void)mouseMoved:(NSEvent*) event
{
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;

    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Distance distance = mc::Distance((int32_t)[event deltaX], (int32_t)[event deltaY]);

    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    auto mouseMovedEvent = MakeRef<Event>(eventDataMap_t{
        { "location", mc::Position((uint32_t)cursorPoint.x, (uint32_t)(contentHeight - cursorPoint.y)) },
        { "distance", distance }
    });

    windowInstance->fireEvent("mouseMoved", mouseMovedEvent);
}

- (void)mouseDragged:(NSEvent*) event
{
    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    NSWindow* window = [self window];
    CGFloat contentHeight = [window contentRectForFrameRect: window.frame].size.height;

    NSPoint cursorPoint = [ event locationInWindow ];
    mc::Position cursorPosition = mc::Position(
        (uint32_t)cursorPoint.x,
        (uint32_t)(contentHeight - cursorPoint.y)
    );

    // Check if the window needs to be
    // manually dragged by the virtual border.
    bool isBorderless = getInternalFlag(
        windowInstance->getWindowFlags(),
        InternalWindowFlag::WindowStyleBorderless
    );

    bool isDraggable = getInternalFlag(
        windowInstance->getWindowFlags(),
        InternalWindowFlag::WindowStyleDraggable
    );

    if (isBorderless && isDraggable) {
        const int32_t virtualBorderWidth = (int32_t)windowInstance->getWidth() - 120;
        const int32_t virtualBorderHeight = 40;

        if (cursorPosition.x > 0 && cursorPosition.x < virtualBorderWidth &&
            cursorPosition.y > 0 && cursorPosition.y < virtualBorderHeight) {

            // Indicate that the window is being dragged
            [self.window performWindowDragWithEvent:event];
            return;
        }
    }

    mc::Distance distance = mc::Distance((int32_t)[event deltaX], (int32_t)[event deltaY]);

    auto mouseMovedEvent = MakeRef<Event>(eventDataMap_t{
        { "location", cursorPosition },
        { "distance", distance }
    });

    windowInstance->fireEvent("mouseMoved", mouseMovedEvent);
}

-(void)keyDown:(NSEvent*) event
{
    NSUInteger flags = [[NSApp currentEvent] modifierFlags];

    bool shiftPressed = (bool)((flags & NSEventModifierFlagShift));
    bool ctrlPressed = (bool)((flags & NSEventModifierFlagCommand)); // ctrl -> command
    bool capslock = (bool)((flags & NSEventModifierFlagCapsLock));
    bool repeated = (bool)[[NSApp currentEvent] isARepeat];

    KeyCode keycode = NativeToMcKeycode([event keyCode]);
    char charRepresentation = McKeycodeToChar(keycode, shiftPressed, capslock);

    auto keyDownEvent = MakeRef<Event>(eventDataMap_t{
        { "keyCode", keycode },
        { "char", charRepresentation },
        { "shiftPressed", shiftPressed },
        { "ctrlPressed", ctrlPressed },
        { "capslockOn", capslock },
        { "repeated", repeated }
    });

    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    windowInstance->fireEvent("keyDown", keyDownEvent);
}

-(void)keyUp:(NSEvent*) event 
{
    KeyCode keycode = NativeToMcKeycode([event keyCode]);
    char charRepresentation = McKeycodeToChar(keycode, false, false);

    auto keyUpEvent = MakeRef<Event>(eventDataMap_t{
        { "keyCode", keycode },
        { "char", charRepresentation }
    });

    OSXNativeWindow* windowInstance = [self mcWindowHandle];
    windowInstance->fireEvent("keyUp", keyUpEvent);
}

@end

@implementation OSXWindowClass
-(BOOL)canBecomeKeyWindow { return YES; }
-(BOOL)canBecomeMainWindow { return YES; }
@end

namespace mc
{
    OSXNativeWindow::OSXNativeWindow(uint64_t windowFlags) {
        d_width = 800;
        d_height = 600;
        d_title = "MacOS Window";
        d_windowFlags = windowFlags;

        // Create the platform native window
        _createOSXWindow(windowFlags);

        // Get the DPI scaling factor
        d_scalingFactor = [d_windowHandle backingScaleFactor];

        // Create the render target
        d_renderTarget = MakeRef<OSXRenderTarget>(d_windowHandle.contentView, d_scalingFactor);

        // Setup event handlers
        on("sizeChanged", [this](Shared<Event> event) {
            auto newWidth = event->get<uint32_t>("width");
            auto newHeight = event->get<uint32_t>("height");

            d_width = newWidth;
            d_height = newHeight;

            d_renderTarget->resize(d_width, d_height);
        });

        on("focusChanged", [this](Shared<Event> event) {
            d_focused = event->get<bool>("focused");
        });

        if (!utils::PlacementConstraintSystem::hasContainer(MAIN_SCREEN_CONTAINER_NAME)) {
            uint32_t screenWidth = (uint32_t)[[NSScreen mainScreen] visibleFrame].size.width;
            uint32_t screenHeight = (uint32_t)[[NSScreen mainScreen] visibleFrame].size.height;

			utils::PlacementConstraintSystem::registerContainer(
				MAIN_SCREEN_CONTAINER_NAME,
				Size(screenWidth, screenHeight)
			);
		}

        // Register the window instance
		registerNativeWindow(this);

        // On the first window creation, initialize mouse hooks
        static bool s_globalMouseHookInitialized = false;
        if (!s_globalMouseHookInitialized) {
            s_globalMouseHookInitialized = true;

            // Installing the global hook to track mouse
            // events outside of the current application.
            [NSEvent addGlobalMonitorForEventsMatchingMask:
                NSEventMaskLeftMouseDown
                handler:^(NSEvent* event
            ) {
                if (event.type == NSEventTypeLeftMouseDown) {
                    NSPoint mouseLocation = [NSEvent mouseLocation];
                    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

                    // Convert the mouse position to
                    // a monochrome Position struct.
                    mc::Position cursorPosition = {
                        (int32_t)mouseLocation.x,
                        (int32_t)(screenHeight - mouseLocation.y)
                    };

                    // Go through each registered window intance
                    // and fire the global mouse clicked event.
                    for (auto window : mc::s_registeredWindows) {
                        window->fireEvent("globalMouseDown", mc::MakeRef<mc::Event>(mc::eventDataMap_t{
                            { "location", cursorPosition },
                            { "button", mc::MouseButton::Left }
                        }));
                    }
                }
            }];

            // Installing the local hook to track mouse
            // events inside the current application.
            [NSEvent addLocalMonitorForEventsMatchingMask:
                NSEventMaskLeftMouseDown
                handler:^NSEvent*(NSEvent* event
            ) {
                if (event.type == NSEventTypeLeftMouseDown) {
                    NSPoint mouseLocation = [NSEvent mouseLocation];
                    int32_t screenHeight = (int32_t)[[NSScreen mainScreen] visibleFrame].size.height;

                    // Convert the mouse position to
                    // a monochrome Position struct.
                    mc::Position cursorPosition = {
                        (int32_t)mouseLocation.x,
                        (int32_t)(screenHeight - mouseLocation.y)
                    };

                    // Go through each registered window intance
                    // and fire the global mouse clicked event.
                    for (auto window : mc::s_registeredWindows) {
                        window->fireEvent("globalMouseDown", mc::MakeRef<mc::Event>(mc::eventDataMap_t{
                            { "location", cursorPosition },
                            { "button", mc::MouseButton::Left }
                        }));
                    }
                }

                return event;
            }];
        }
    }

    void OSXNativeWindow::show() {
        CORE_ASSERT(!d_visible, "Window is already opened, you called 'show()' twice");

        d_visible = true;
        [this->d_windowHandle orderFront:nil];
    }

    void OSXNativeWindow::hide() {
        CORE_ASSERT(d_visible, "Window is already hidden, you called 'hide()' twice");

        d_visible = false;
        [this->d_windowHandle orderOut:nil];
    }

    void OSXNativeWindow::focus() {
        [this->d_windowHandle makeKeyAndOrderFront:nil];
    }

    void OSXNativeWindow::unfocus() {
        [this->d_windowHandle resignKeyWindow];
    }

    void OSXNativeWindow::close() {
        if([[d_windowHandle delegate] respondsToSelector:@selector(windowShouldClose:)]) {
            if(![[d_windowHandle delegate] windowShouldClose:d_windowHandle]) {
                return;
            }
        }

        [d_windowHandle close];
    }

    void OSXNativeWindow::maximize() {
        [d_windowHandle zoom:d_windowHandle];
    }
    
    void OSXNativeWindow::restoreMaximize() {
        [d_windowHandle zoom:d_windowHandle];
    }

    void OSXNativeWindow::minimize() {
        [this->d_windowHandle miniaturize:d_windowHandle];
    }

    void OSXNativeWindow::setWidth(uint32_t width) {
        d_width = width;
        _setWindowSize(d_width, d_height);
    }

    void OSXNativeWindow::setHeight(uint32_t height) {
        d_height = height;
        _setWindowSize(d_width, d_height);
    }

    void OSXNativeWindow::setPosition(const Position& pos) {
        d_position = pos;

        float topLeftOrigin =   [[NSScreen mainScreen] visibleFrame].origin.y +
                                [[NSScreen mainScreen] visibleFrame].size.height;

        [this->d_windowHandle setFrameTopLeftPoint: CGPointMake(pos.x, (int32_t)topLeftOrigin - pos.y)];
    }

    void OSXNativeWindow::setTitle(const std::string& title) {
        d_title = title;
        [d_windowHandle setTitle:[NSString stringWithUTF8String: d_title.c_str()]];
    }

    void OSXNativeWindow::_createOSXWindow(uint64_t windowFlags) {
        bool isBorderless = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleBorderless);
        bool isClosable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleClosable);
        bool isMiniaturizable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleMinimizable);
        bool isMaximizable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleMaximizable);
        bool isResizable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleResizable);
        bool isTitled = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleTitled);

        NSWindowStyleMask windowStyleMask = 0;

        // If the borderless flag is set,
        // override all other window styles.
        if (isBorderless) {
            windowStyleMask = NSWindowStyleMaskBorderless;
        }

        // Resziable check comes after because
        // borderless windows can still be resized.
        if (isResizable) {
            windowStyleMask |= NSWindowStyleMaskResizable;
        }

        if (isClosable) windowStyleMask |= NSWindowStyleMaskClosable;
        if (isMiniaturizable) windowStyleMask |= NSWindowStyleMaskMiniaturizable;
        if (isTitled) windowStyleMask |= NSWindowStyleMaskTitled;

        d_windowHandle = [[OSXWindowClass alloc] initWithContentRect:NSMakeRect(100, 300, d_width, d_height)
                                                            styleMask: windowStyleMask
                                                            backing:NSBackingStoreBuffered defer:NO];

        // Special case for hiding 'zoom', 'minimize', or 'close'
        // buttons if the window should not be maximiable.
        if (!isClosable) {
            [[d_windowHandle standardWindowButton:NSWindowCloseButton] setHidden:YES];
        }

        if (!isMiniaturizable) {
            [[d_windowHandle standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
        }

        if (!isMaximizable) {
            [[d_windowHandle standardWindowButton:NSWindowZoomButton] setHidden:YES];
        }

        OSXWindowDelegate* windowDelegate = [[OSXWindowDelegate alloc] init];
        [d_windowHandle setDelegate:(id)windowDelegate];
        windowDelegate.mcWindowHandle = this;

        [d_windowHandle setAcceptsMouseMovedEvents:YES];
        [d_windowHandle setTitle:[NSString stringWithUTF8String: d_title.c_str()]];

        // Content View
        OSXWindowContentViewDelegate* contentViewDelegate = [[OSXWindowContentViewDelegate alloc] initWithFrame:NSMakeRect(0, 0, d_width, d_height)];
        contentViewDelegate.mcWindowHandle = this;
        [d_windowHandle setContentView:contentViewDelegate];

        // Sets the refresh rate of the content view
        [NSTimer scheduledTimerWithTimeInterval:1.0/60.0 target:contentViewDelegate selector:@selector(update) userInfo:nil repeats:YES];
    }

    void OSXNativeWindow::_setWindowSize(uint32_t width, uint32_t height) {
        NSRect frame = [d_windowHandle frame];
        
        // Adjust for the bottom left origin point
        frame.origin.y -= frame.size.height;
        frame.origin.y += height;

        frame.size = NSMakeSize(CGFloat(width), CGFloat(height));
        [d_windowHandle setFrame: frame display: YES animate: NO];

        // Update render target size
        d_renderTarget->resize(d_width, d_height);

        // Fire the sizeChanged event
        auto resizeEvent = MakeRef<Event>(eventDataMap_t{
            { "width", width },
            { "height", height }
        });

        fireEvent("sizeChanged", resizeEvent);
    }
} // namespace mc
