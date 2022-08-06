#include "Win32NativeWindow.h"
#include "Win32ApplicationContext.h"
#include <core/InternalFlags.h>
#include <widgets/Layer.h>
#include <events/MouseEvents.h>
#include <events/KeyboardEvents.h>
#include <utils/uuid.h>
#include <utils/Cursor.h>
#include <utils/PlacementConstraintSystem.h>
#include <mutex>

typedef BOOL(__stdcall* SetProcessDpiAwarenessContextFn)(DPI_AWARENESS_CONTEXT);
LRESULT CALLBACK setupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK msgRedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HHOOK s_hMouseHook = NULL;
LRESULT CALLBACK win32LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

#define GET_X_LPARAM(lp)    static_cast<int>((int16_t)LOWORD(lp))
#define GET_Y_LPARAM(lp)    static_cast<int>((int16_t)HIWORD(lp))

namespace mc {
    static std::mutex s_windowMapMutex;
    static std::vector<Win32NativeWindow*> s_registeredWindows;

    static void registerNativeWindow(Win32NativeWindow* window) {
        s_windowMapMutex.lock();
        s_registeredWindows.push_back(window);
        s_windowMapMutex.unlock();
    }

    static void unregisterNativeWindow(Win32NativeWindow* window) {
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

    Win32NativeWindow::Win32NativeWindow(uint64_t windowFlags) {
        d_width = 800;
        d_height = 600;
        d_title = "Win32 Window";
        d_windowFlags = windowFlags;

        // Create the platform native window
        _createWin32Window(windowFlags);

        // Create the render target
        d_renderTarget = MakeRef<Win32RenderTarget>(d_windowHandle, d_scalingFactor);

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
            uint32_t screenWidth = (uint32_t)GetSystemMetrics(SM_CXSCREEN);
            uint32_t screenHeight = (uint32_t)GetSystemMetrics(SM_CYSCREEN);

            utils::PlacementConstraintSystem::registerContainer(
                MAIN_SCREEN_CONTAINER_NAME,
                Size(screenWidth, screenHeight)
            );
        }

        // Create the global low-level mouse hook
        if (!s_hMouseHook) {
            /*s_hMouseHook = SetWindowsHookEx(
                WH_MOUSE_LL,
                win32LowLevelMouseProc,
                GetModuleHandle(NULL),
                0
            );*/
        }

        // Register the window instance
        registerNativeWindow(this);
    }

    void Win32NativeWindow::setWidth(uint32_t width) {
        d_width = width;
        _setWindowSize(d_width, d_height);
    }

    void Win32NativeWindow::setHeight(uint32_t height) {
        d_height = height;
        _setWindowSize(d_width, d_height);
    }

    void Win32NativeWindow::setMinWidth(uint32_t minWidth) {
        d_minSize.width = minWidth;
    }

    void Win32NativeWindow::setMaxWidth(uint32_t maxWidth) {
        d_maxSize.width = maxWidth;
    }

    void Win32NativeWindow::setMinHeight(uint32_t minHeight) {
        d_minSize.height = minHeight;
    }

    void Win32NativeWindow::setMaxHeight(uint32_t maxHeight) {
        d_maxSize.height = maxHeight;
    }

    void Win32NativeWindow::setPosition(const Position& pos) {
        d_position = pos;

        // Get window's current position and size
        RECT rect;
        GetWindowRect(d_windowHandle, &rect);

        int width = static_cast<int>(rect.right - rect.left);
        int height = static_cast<int>(rect.bottom - rect.top);

        // Set new window position
        SetWindowPos(d_windowHandle, NULL, pos.x, pos.y, width, height, 0);
    }

    void Win32NativeWindow::setTitle(const std::string& title) {
        d_title = title;
        ::SetWindowTextA(d_windowHandle, title.c_str());
    }

    void Win32NativeWindow::show() {
        CORE_ASSERT(!d_visible, "Window is already opened, you called 'show()' twice");

        d_visible = true;
        ::ShowWindow(d_windowHandle, SW_SHOW);
    }

    void Win32NativeWindow::hide() {
        CORE_ASSERT(d_visible, "Window is already hidden, you called 'hide()' twice");

        d_visible = false;
        ::ShowWindow(d_windowHandle, SW_HIDE);
    }

    void Win32NativeWindow::focus() {
        ::SetFocus(d_windowHandle);
    }

    void Win32NativeWindow::unfocus() {
        PostMessage(d_windowHandle, WM_ACTIVATE, 0, 0);
    }

    void Win32NativeWindow::close() {
        PostMessage(d_windowHandle, WM_CLOSE, 0, 0);
    }

    void Win32NativeWindow::maximize() {
        ::ShowWindow(d_windowHandle, SW_MAXIMIZE);
    }

    void Win32NativeWindow::restoreMaximize() {
        ::ShowWindow(d_windowHandle, SW_RESTORE);
    }

    void Win32NativeWindow::minimize() {
        ::ShowWindow(d_windowHandle, SW_MINIMIZE);
    }

    void Win32NativeWindow::requestRedraw() {
        ::SendMessage(d_windowHandle, WM_PAINT, NULL, NULL);
    }

    void Win32NativeWindow::_createWin32Window(uint64_t windowFlags) {
        // Extracting internal window flags
        bool isBorderless = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleBorderless);
        bool isClosable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleClosable);

        bool isMinimizable =
            getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleMinimizable);
        bool isMaximizable =
            getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleMaximizable);

        bool isResizable = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleResizable);
        bool isTitled = getInternalFlag(windowFlags, InternalWindowFlag::WindowStyleTitled);

#pragma warning(push)
#pragma warning(disable: 6387)
        SetProcessDpiAwarenessContextFn dpiAwarenessProc =
            (SetProcessDpiAwarenessContextFn)GetProcAddress(
                LoadLibraryA("User32.dll"),
                "SetProcessDpiAwarenessContext"
            );

        // Set window DPI-Awareness mode to be aware of high dpi displays
        if (dpiAwarenessProc) {
            dpiAwarenessProc(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
        }
#pragma warning(pop)

        std::string windowClassName = std::to_string(utils::generateUUID());

        WNDCLASSEXA wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEXA));
        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.cbClsExtra = NULL;
        wc.cbWndExtra = NULL;
        wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
        wc.hInstance = NULL;
        wc.lpszClassName = windowClassName.c_str();
        wc.lpszMenuName = "";
        wc.lpfnWndProc = setupWindowProc;
        wc.style = NULL;

        // Registering Win32 API Class
        if (!RegisterClassExA(&wc)) {
            CORE_ASSERT(false, "Failed to register Win32 window class");
            return;
        }

        // Creating window rect
        RECT windowFrameRect;
        windowFrameRect.left = 100;
        windowFrameRect.top = 300;
        windowFrameRect.right = windowFrameRect.left + d_width;
        windowFrameRect.bottom = windowFrameRect.top + d_height;

        DWORD dwStyle = WS_OVERLAPPED;
        DWORD dwExStyle = WS_EX_OVERLAPPEDWINDOW;

        if (isMinimizable)  dwStyle |= WS_MINIMIZEBOX;
        if (isMaximizable)  dwStyle |= WS_MAXIMIZEBOX;
        if (isResizable)    dwStyle |= WS_THICKFRAME;
        if (isTitled)       dwStyle |= (WS_SYSMENU | WS_CAPTION);

        if (isBorderless) {
            dwStyle = WS_POPUP;
            dwExStyle = WS_EX_APPWINDOW;
        }

        // Adjusting window rect to be exact size that we specified depending on the window style
        ::AdjustWindowRect(&windowFrameRect, dwStyle, FALSE);

        // Creating window handle
        d_windowHandle = ::CreateWindowExA(
            dwExStyle,
            windowClassName.c_str(),
            d_title.c_str(),
            dwStyle,
            windowFrameRect.left,
            windowFrameRect.top,
            windowFrameRect.right - windowFrameRect.left,
            windowFrameRect.bottom - windowFrameRect.top,
            NULL, NULL, NULL,

            /* This parameter is crucial as it specifies the
            user data pointer to be used in the WindowProc */
            this
        );

        // Special case for enabling and disable the 'close' button
        if (!isClosable) {
            EnableMenuItem(GetSystemMenu(d_windowHandle, FALSE), SC_CLOSE,
                MF_BYCOMMAND | MF_DISABLED);
        } else {
            EnableMenuItem(GetSystemMenu(d_windowHandle, FALSE), SC_CLOSE,
                MF_BYCOMMAND | MF_ENABLED);
        }

        CORE_ASSERT(d_windowHandle, "Failed to create Win32 window, HWND is nullptr");

        // Retrieve the window's DPI scaling factor
        d_scalingFactor = static_cast<float>(GetDpiForWindow(d_windowHandle)) / 96.0f;

        // Update the native platform window
        ::UpdateWindow(d_windowHandle);

        // Set the native window handle of the Win32 application context
        std::static_pointer_cast<Win32ApplicationContext>(
            d_applicationContext
        )->registerWin32NativeWindowHandle(this);
    }

    void Win32NativeWindow::_setWindowSize(uint32_t width, uint32_t height) {
        // Get window's current position and size
        RECT rect;
        ::GetWindowRect(d_windowHandle, &rect);

        // Set new window size
        ::SetWindowPos(d_windowHandle, NULL, rect.left, rect.top, width, height, 0);

        // Update render target size
        d_renderTarget->resize(d_width, d_height);
    }

    void Win32NativeWindow::destroyPlatformWindow() {
        ::DestroyWindow(d_windowHandle);
    }

    LRESULT
    CALLBACK
    Win32NativeWindow::win32WindowProcCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        // If there is an update callback from a native window owner, execute it.
        auto updateCallback = getUpdateCallback();

        if (updateCallback) {
            updateCallback();
        }

        // Process the received message
        switch (uMsg) {
        case WM_CLOSE: {
            unregisterNativeWindow(this);

            // Stop the application only if the
            // destroyed window was the root window.
            if (isRoot()) {
                std::static_pointer_cast<Win32ApplicationContext>(
                    d_applicationContext
                )->mainWindowRequestedClose();
            }
            break;
        }
        case WM_MOVE: {
            HMONITOR monitor = MonitorFromWindow(d_windowHandle, MONITOR_DEFAULTTONEAREST);

            MONITORINFO info;
            info.cbSize = sizeof(info);
            if (GetMonitorInfo(monitor, &info)) {
                uint32_t screenWidth = std::abs(info.rcMonitor.left - info.rcMonitor.right);
                uint32_t screenHeight = std::abs(info.rcMonitor.top - info.rcMonitor.bottom);

                utils::PlacementConstraintSystem::updateContainer(
                    MAIN_SCREEN_CONTAINER_NAME,
                    Position(info.rcMonitor.left, info.rcMonitor.top),
                    Size(screenWidth, screenHeight)
                );
            }

            break;
        }
        case WM_SIZE: {
            uint32_t newWidth = static_cast<uint32_t>(LOWORD(lParam));
            uint32_t newHeight = static_cast<uint32_t>(HIWORD(lParam));

            // Fire the window resized event
            auto resizeEvent = MakeRef<Event>(eventDataMap_t{
                { "width", newWidth },
                { "height", newHeight }
            });

            fireEvent("sizeChanged", resizeEvent);

            // While the window is resizing, the contents of
            // the window should be rendered on the main thread.
            requestFrontBufferRender();
            break;
        }
        case WM_GETMINMAXINFO: {
            LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
            lpMMI->ptMinTrackSize.x = static_cast<LONG>(d_minSize.width);
            lpMMI->ptMinTrackSize.y = static_cast<LONG>(d_minSize.height);
            lpMMI->ptMaxTrackSize.x = static_cast<LONG>(d_maxSize.width);
            lpMMI->ptMaxTrackSize.y = static_cast<LONG>(d_maxSize.height);
            break;
        }
        case WM_NCHITTEST: {
            // Call the default window procedure for default handling.
            const LRESULT result = DefWindowProc(hwnd, uMsg, wParam, lParam);

            // Check if the window is borderless and requires manual resizing/dragging
            bool isBorderless =
                getInternalFlag(d_windowFlags, InternalWindowFlag::WindowStyleBorderless);

            bool isResizable =
                getInternalFlag(d_windowFlags, InternalWindowFlag::WindowStyleResizable);

            bool isDraggable =
                getInternalFlag(d_windowFlags, InternalWindowFlag::WindowStyleDraggable);

            if (isBorderless) {
                // Get the location of the mouse click, which is packed into lParam.
                POINT pt;
                pt.x = GET_X_LPARAM(lParam);
                pt.y = GET_Y_LPARAM(lParam);

                if (isResizable) {
                    const auto CheckForResizingConditions =
                        [this](POINT pt, LONG BorderWidth, LRESULT& result) -> bool {
                        RECT ClientArea;
                        GetClientRect(d_windowHandle, &ClientArea);
                        ScreenToClient(d_windowHandle, &pt);

                        /*top-left, top and top-right*/
                        if (pt.y < BorderWidth) {
                            if (pt.x < BorderWidth) {
                                result = HTTOPLEFT;
                                return true;
                            } else if (pt.x > (ClientArea.right - BorderWidth)) {
                                result = HTTOPRIGHT;
                                return true;
                            }
                            result = HTTOP;
                            return true;
                        }
                        /*bottom-left, bottom and bottom-right */
                        if (pt.y > (ClientArea.bottom - BorderWidth)) {
                            if (pt.x < BorderWidth) {
                                result = HTBOTTOMLEFT;
                                return true;
                            } else if (pt.x > (ClientArea.right - BorderWidth)) {
                                result = HTBOTTOMRIGHT;
                                return true;
                            }

                            result = HTBOTTOM;
                            return true;
                        }
                        if (pt.x < BorderWidth) {
                            result = HTLEFT;
                            return true;
                        }
                        if (pt.x > (ClientArea.right - BorderWidth)) {
                            result = HTRIGHT;
                            return true;
                        }

                        return false;
                    };

                    LRESULT ResizingConditionsResult = 0;
                    const LONG BorderWidth = 10;
                    if (CheckForResizingConditions(pt, BorderWidth, ResizingConditionsResult))
                        return ResizingConditionsResult;
                }

                // Check if window needs to be manually dragged by the border
                if (isDraggable) {
                    const LONG virtualBorderWidth = d_width - 40 * 3;
                    const LONG virtualBorderHeight = 40;

                    RECT rcDraggable = { 0, 0, virtualBorderWidth, virtualBorderHeight };

                    MapWindowPoints(
                        hwnd,
                        NULL,
                        reinterpret_cast<POINT*>(&rcDraggable),
                        (sizeof(RECT) / sizeof(POINT))
                    );

                    if ((result == HTCLIENT) && (PtInRect(&rcDraggable, pt))) {
                        return HTCAPTION;
                    }
                }
            }

            break;
        }
        case WM_ACTIVATE: {
            uint16_t focusFlag = LOWORD(wParam);
            bool isWindowFocused = (focusFlag == WA_ACTIVE) || (focusFlag == WA_CLICKACTIVE);

            // Ignore the event in the case when the window isn't visible
            // and said to have lost focus (usually occurs at startup).
            if (!d_visible && !isWindowFocused) {
                break;
            }

            auto focusChangedEvent = MakeRef<Event>(eventDataMap_t{
                { "focused", isWindowFocused }
            });

            fireEvent("focusChanged", focusChangedEvent);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseDownEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Left }
            });

            fireEvent("mouseDown", mouseDownEvent);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseDownEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Right }
            });

            fireEvent("mouseDown", mouseDownEvent);
            break;
        }
        case WM_MBUTTONDOWN:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseDownEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Middle }
            });

            fireEvent("mouseDown", mouseDownEvent);
            break;
        }
        case WM_LBUTTONUP:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseUpEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Left }
            });

            fireEvent("mouseUp", mouseUpEvent);
            break;
        }
        case WM_RBUTTONUP:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseUpEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Right }
            });

            fireEvent("mouseUp", mouseUpEvent);
            break;
        }
        case WM_MBUTTONUP:
        {
            POINT windowCursorPoint = POINT{
                (LONG)GET_X_LPARAM(lParam),
                (LONG)GET_Y_LPARAM(lParam)
            };

            Position cursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            ClientToScreen(hwnd, &windowCursorPoint);
            Position screenCursorPoint = Position(
                windowCursorPoint.x,
                windowCursorPoint.y
            );

            auto mouseUpEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "screenLocation", screenCursorPoint },
                { "button", MouseButton::Middle }
            });

            fireEvent("mouseUp", mouseUpEvent);
            break;
        }
        case WM_MOUSEMOVE:
        {
            Position cursorPoint = Position(
                (uint32_t)GET_X_LPARAM(lParam),
                (uint32_t)GET_Y_LPARAM(lParam)
            );

            Distance distance = Distance(
                cursorPoint.x - d_previousMousePosition.x,
                cursorPoint.y - d_previousMousePosition.y
            );

            auto mouseMovedEvent = MakeRef<Event>(eventDataMap_t{
                { "location", cursorPoint },
                { "distance", distance }
            });

            fireEvent("mouseMoved", mouseMovedEvent);

            d_previousMousePosition = cursorPoint;

            // Tracking the mouse leaving and entering the window
            // ** Check out WM_MOUSELEAVE message **
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(tme);
            tme.hwndTrack = d_windowHandle;
            tme.dwFlags = TME_HOVER | TME_LEAVE;
            tme.dwHoverTime = HOVER_DEFAULT;
            TrackMouseEvent(&tme);
            break;
        }
        case WM_KEYDOWN:
        {
            int vkKeycode = static_cast<int>(wParam);
            bool shiftPressed = static_cast<bool>((GetKeyState(VK_SHIFT) & 0x8000));
            bool ctrlPressed = static_cast<bool>(
                GetKeyState(VK_LCONTROL) & 0x8000 ||
                GetKeyState(VK_RCONTROL) & 0x8000
            );
            bool capslock = static_cast<bool>((GetKeyState(VK_CAPITAL) & 0x0001));
            bool repeated = static_cast<bool>((HIWORD(lParam) & KF_REPEAT));
            KeyCode keycode = NativeToMcKeycode(vkKeycode);
            char charRepresentation = McKeycodeToChar(keycode, shiftPressed, capslock);

            auto keyDownEvent = MakeRef<Event>(eventDataMap_t{
                { "keyCode", keycode },
                { "char", charRepresentation },
                { "shiftPressed", shiftPressed },
                { "ctrlPressed", ctrlPressed },
                { "capslockOn", capslock },
                { "repeated", repeated }
            });

            fireEvent("keyDown", keyDownEvent);
            break;
        }
        case WM_KEYUP:
        {
            int vkKeycode = static_cast<int>(wParam);
            KeyCode keycode = NativeToMcKeycode(vkKeycode);
            char charRepresentation = McKeycodeToChar(keycode, false, false);

            auto keyUpEvent = MakeRef<Event>(eventDataMap_t{
                { "keyCode", keycode },
                { "char", charRepresentation }
            });

            fireEvent("keyUp", keyUpEvent);
            break;
        }
        case WM_SETCURSOR: {
            auto activeCursor = static_cast<LPCWSTR>(utils::Cursor::getActiveCursor());
            SetCursor(LoadCursorW(NULL, activeCursor));
            break;
        }
        case WM_MOUSELEAVE: {
            //
            // *** Potential Development ***
            // Currently not supported on MacOS or Linux
            //
            //fireEvent("mouseLeftWindow", Event::empty);
            break;
        }
        default: break;
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
} // namespace mc

LRESULT CALLBACK setupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_NCCREATE: {
        // Retrieves Win32NativeWindow pointer from the initial createstruct
        // data and assigns it to the GWLP_USERDATA so it can be accessed
        // later from the main running WindowProc. It also sets MsgRedirectWindowProc
        // as the new primary window procedure.

        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        mc::Win32NativeWindow* pWindow =
            static_cast<mc::Win32NativeWindow*>(pCreate->lpCreateParams);

        CORE_ASSERT(pWindow, "Critical Error: Pointer to WindowData is null during WM_NCCREATE");

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(msgRedirectWindowProc));
        return pWindow->win32WindowProcCallback(hwnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT msgRedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Retrieves WindowsWindow pointer from the GWL_USERDATA
    // and calls the WindowsWindow's own window procedure.
    mc::Win32NativeWindow* pWindow =
        reinterpret_cast<mc::Win32NativeWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    return pWindow->win32WindowProcCallback(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK win32LowLevelMouseProc(
    int    nCode,
    WPARAM wParam,
    LPARAM lParam
) {
    MOUSEHOOKSTRUCT* pMouseStruct = reinterpret_cast<MOUSEHOOKSTRUCT*>(lParam);
    if (pMouseStruct != NULL) {
        if (wParam == WM_LBUTTONDOWN) {
            // Convert the mouse position to
            // a monochrome Position struct.
            mc::Position cursorPosition = {
                (int32_t)pMouseStruct->pt.x,
                (int32_t)pMouseStruct->pt.y
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
    }

    return CallNextHookEx(s_hMouseHook, nCode, wParam, lParam);
}
