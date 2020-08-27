#include "LinuxWindow.h"
#include <graphics/Graphics.h>
#include <ui/UIElements.h>
#include <events/Events.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <locale>
#include <codecvt>
#include <unistd.h>

namespace mc
{
    typedef struct _LinuxGraphicsNativeInformation
    {
        int         id;
        Display*    display;
        int         screen;
        Visual*     visual;
        Window      window;
        int 		window_width;
		int			window_height;
    } LinuxGraphicsNativeInformation;

    static std::vector<Window> s_RegisteredWindows;
    static LinuxWindow* s_CurrentActiveWindowInstance = nullptr;
    static size_t s_ActiveWindowIndex = 0;

    static Point _mc_LinuxWindow_static_previous_mouse_position_ = { 0, 0 };
    static Point _mc_LinuxWindow_static_drag_panel_click_position = { 0, 0 };

    static bool ModernWindowCloseButton_OnMouseClick(Event& event, UIView* sender)
    {
        size_t window_idx = dynamic_cast<LinuxWindow*>(sender->srcwindow)->m_RegisteredWindowIndex;
        Window handle = s_RegisteredWindows[window_idx];
        Display* display = dynamic_cast<LinuxWindow*>(sender->srcwindow)->m_Display;

        XEvent evtclose;
        evtclose.xclient.type = ClientMessage;
        evtclose.xclient.window = handle;
        evtclose.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", True);
        evtclose.xclient.format = 32;
        evtclose.xclient.data.l[0] = XInternAtom(display, "WM_DELETE_WINDOW", False);
        evtclose.xclient.data.l[1] = CurrentTime;
        XSendEvent(display, handle, False, NoEventMask, &evtclose);
        return EVENT_HANDLED;
    }

    static bool ModernWindowMaximizeButton_OnMouseClick(Event& event, UIView* sender)
    {

        return EVENT_HANDLED;
    }

    static bool ModernWindowMinimizeButton_OnMouseClick(Event& event, UIView* sender)
    {

        return EVENT_HANDLED;
    }

    static void PerformDragPanelCollision(Display* display, Window window, Size position)
    {
        XWindowAttributes xwa;
        XGetWindowAttributes(display, window, &xwa);

        Size delta = {
            position.x - _mc_LinuxWindow_static_drag_panel_click_position.x,
            position.y - _mc_LinuxWindow_static_drag_panel_click_position.y
        };

        XMoveWindow(display, window, (int)(xwa.x + delta.x), (int)(xwa.y + delta.y));

        _mc_LinuxWindow_static_drag_panel_click_position = position;
    }

    UIWindow* UIWindow::GetCurrentActiveWindow()
    {
        return s_CurrentActiveWindowInstance;
    }

    Ref<UIWindow> UIWindow::Create(WindowStyle style, uint32_t width, uint32_t height, const char* title)
    {
        return Ref<UIWindow>(new LinuxWindow(style, width, height, title));
    }

    LinuxWindow::LinuxWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title)
    {
        m_WindowStyle = WindowStyle::Classic; // modern style is not supported yet
        m_Width = width;
        m_Height = height;
        m_Title = title;
        Init();
    }

    void LinuxWindow::Init()
    {
        m_Display = XOpenDisplay(NULL);
        if (!m_Display)
        {
            printf("Failed to open display\n");
            return;
        }

        m_Screen = DefaultScreen(m_Display);
        m_Visual = (void*)DefaultVisual(m_Display, m_Screen);

        Window window = XCreateSimpleWindow(
            m_Display,
            RootWindow(m_Display, m_Screen),
            200, 200, m_Width, m_Height, 1,
            BlackPixel(m_Display, m_Screen),
            WhitePixel(m_Display, m_Screen)
        );

        m_RegisteredWindowIndex = s_RegisteredWindows.size();
        s_RegisteredWindows.push_back(window);

        int mask =  ExposureMask | KeyPressMask | ButtonPressMask |
                    StructureNotifyMask | ButtonReleaseMask |
                    KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
                    PointerMotionMask | Button1MotionMask | VisibilityChangeMask |
                    ColormapChangeMask | FocusChangeMask;

        if (m_WindowStyle == WindowStyle::Modern)
        {
            Atom window_type = XInternAtom(m_Display, "_NET_WM_WINDOW_TYPE", False);
            long value = XInternAtom(m_Display, "_NET_WM_WINDOW_TYPE_DOCK", False);
            XChangeProperty(m_Display, window, window_type, XA_ATOM, 32, PropModeReplace, (unsigned char*)&value, 1);
        }

        XSelectInput(m_Display, window, mask);
        XMapWindow(m_Display, window);

        Atom WM_DELETE_WINDOW = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, window, &WM_DELETE_WINDOW, 1);

        LinuxGraphicsNativeInformation init_info;
        init_info.id = m_RegisteredWindowIndex;
        init_info.display = m_Display;
        init_info.screen = m_Screen;
        init_info.visual = reinterpret_cast<Visual*>(m_Visual);
        init_info.window = window;
        init_info.window_width = m_Width;
        init_info.window_height = m_Height;
        Graphics::Initialize(&init_info);
        Graphics::SetActiveTarget(&init_info);

        // If the window is of modern style
		if (m_WindowStyle == WindowStyle::Modern)
		{
			// Setup modern window ui elements
			SetupModernWindowViews();
		}

        s_ActiveWindowIndex = m_RegisteredWindowIndex;
        m_IsOpened = true;
    }

    void LinuxWindow::SetupModernWindowViews()
    {
        m_ModernWindowDragPanel = MakeRef<UIView>();
        m_ModernWindowDragPanel->layer.frame = Frame(0, 0, (float)m_Width - 160, 60);
        m_ModernWindowDragPanel->layer.color = Color::transparent;
        m_ModernWindowDragPanel->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) -> bool {
            _mc_LinuxWindow_static_drag_panel_click_position = sender->srcwindow->GetAsboluteMouseCursorPos();
            return EVENT_HANDLED;
        });
        m_ModernWindowDragPanel->SetZIndex(10000);
        AddView(m_ModernWindowDragPanel);

        m_ModernWindowCloseButton = MakeRef<UIButton>();
        m_ModernWindowCloseButton->layer.frame = Frame((float)m_Width - 46, 0, 46, 36);
        m_ModernWindowCloseButton->CornerRadius = 0;
        m_ModernWindowCloseButton->Label->Text = "X";
        m_ModernWindowCloseButton->HoverOnColor = Color::red;
        m_ModernWindowCloseButton->SetZIndex(20000);
        m_ModernWindowCloseButton->AddEventHandler<EventType::MouseButtonReleased>(ModernWindowCloseButton_OnMouseClick);
        AddView(CastToUiView(m_ModernWindowCloseButton));

        m_ModernWindowMaximizeButton = MakeRef<UIButton>();
        m_ModernWindowMaximizeButton->layer.frame = Frame((float)m_Width - 46 * 2, 0, 46, 36);
        m_ModernWindowMaximizeButton->CornerRadius = 0;
        m_ModernWindowMaximizeButton->Label->UseWidestringText = true;
        m_ModernWindowMaximizeButton->Label->WidestringText = L"⬜";
        m_ModernWindowMaximizeButton->SetZIndex(20000);
        m_ModernWindowMaximizeButton->AddEventHandler<EventType::MouseButtonReleased>(ModernWindowMaximizeButton_OnMouseClick);
        AddView(CastToUiView(m_ModernWindowMaximizeButton));

        m_ModernWindowMinimizeButton = MakeRef<UIButton>();
        m_ModernWindowMinimizeButton->layer.frame = Frame((float)m_Width - 46 * 3, 0, 46, 36);
        m_ModernWindowMinimizeButton->CornerRadius = 0;
        m_ModernWindowMinimizeButton->Label->Text = "_";
        m_ModernWindowMinimizeButton->SetZIndex(20000);
        m_ModernWindowMinimizeButton->AddEventHandler<EventType::MouseButtonReleased>(ModernWindowMinimizeButton_OnMouseClick);
        AddView(CastToUiView(m_ModernWindowMinimizeButton));

        m_ModernWindowTitleLabel = MakeRef<UILabel>();
        m_ModernWindowTitleLabel->layer.frame = Frame(10, 8, 300, 26);
        m_ModernWindowTitleLabel->layer.color = Color::black;
        m_ModernWindowTitleLabel->Text = m_Title;
        m_ModernWindowTitleLabel->Properties.Allignment = TextAlignment::LEADING;
        m_ModernWindowTitleLabel->color = Color::white;
        m_ModernWindowTitleLabel->SetZIndex(10000);
        AddView(CastToUiView(m_ModernWindowTitleLabel));
    }

    void LinuxWindow::AdjustModernWindowViews()
    {
        if (m_ModernWindowDragPanel)
            m_ModernWindowDragPanel->layer.frame = Frame(0, 0, (float)m_Width - 160, 60);

        if (m_ModernWindowCloseButton)
            m_ModernWindowCloseButton->layer.frame = Frame((float)m_Width - 46, 0, 46, 36);

        if (m_ModernWindowMaximizeButton)
            m_ModernWindowMaximizeButton->layer.frame = Frame((float)m_Width - 46 * 2, 0, 46, 36);

        if (m_ModernWindowMinimizeButton)
            m_ModernWindowMinimizeButton->layer.frame = Frame((float)m_Width - 46 * 3, 0, 46, 36);

        if (m_ModernWindowTitleLabel)
            m_ModernWindowTitleLabel->layer.frame = Frame(10, 8, 300, 26);
    }

    void LinuxWindow::ProcessEvents(void* evt)
    {
        XEvent& event = *(XEvent*)evt;
        switch (event.type)
        {
        case ClientMessage:
        {
            m_IsOpened = false;
            return;
        }
        case FocusIn:
        {
            s_ActiveWindowIndex = m_RegisteredWindowIndex;
            break;
        }
        case ConfigureNotify:
        {
            XConfigureEvent& xce = reinterpret_cast<XConfigureEvent&>(event);
            if (xce.width != m_Width || xce.height != m_Height)
            {
                m_Width = xce.width;
                m_Height = xce.height;

                LinuxGraphicsNativeInformation info;
                info.id = m_RegisteredWindowIndex;
                info.display = m_Display;
                info.screen = m_Screen;
                info.visual = reinterpret_cast<Visual*>(m_Visual);
                info.window = s_RegisteredWindows[m_RegisteredWindowIndex];
                info.window_width = m_Width;
                info.window_height = m_Height;
                Graphics::ResizeRenderTarget(&info);
            }
            break;
        }
        case ButtonPress:
        {
            XButtonEvent& be = reinterpret_cast<XButtonEvent&>(event);
            MouseButton button = MouseButton::None;
            switch (be.button)
            {
            case Button1: { button = MouseButton::Left; break; }
            case Button2: { button = MouseButton::Right; break; }
            case Button3: { button = MouseButton::Middle; break; }
            default: break;
            }

			auto e = std::make_shared<MouseButtonPressedEvent>(Point{ (float)be.x, (float)be.y }, button);
			m_SceneManager.DispatchEvent(e);
            break;
        }
        case ButtonRelease:
        {
            XButtonEvent& be = reinterpret_cast<XButtonEvent&>(event.xbutton);
            MouseButton button = MouseButton::None;
            switch (be.button)
            {
            case Button1: { button = MouseButton::Left; break; }
            case Button2: { button = MouseButton::Middle; break; }
            case Button3: { button = MouseButton::Right; break; }
            case Button4:
            {
                // Mouse wheel scrolled up
                auto e = std::make_shared<MouseScrolledEvent>(Point{ (float)be.x, (float)be.y }, 1.0f * m_Dpi);
    			m_SceneManager.DispatchEvent(e);
                return;
            }
            case Button5:
            {
                // Mouse wheel scrolled down
                auto e = std::make_shared<MouseScrolledEvent>(Point{ (float)be.x, (float)be.y }, -1.0f* m_Dpi);
    			m_SceneManager.DispatchEvent(e);
                return;
            }
            default: break;
            }

			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)be.x, (float)be.y }, button);
			m_SceneManager.DispatchEvent(e);
            break;
        }
        case MotionNotify:
        {
            XMotionEvent& motion_e = reinterpret_cast<XMotionEvent&>(event.xmotion);

            auto click_location = Point{ (float)motion_e.x, (float)motion_e.y };
            auto distance = Point {
                click_location.x - (float)_mc_LinuxWindow_static_previous_mouse_position_.x,
                click_location.y - (float)_mc_LinuxWindow_static_previous_mouse_position_.y
            };

            MouseButton pressed_button = MouseButton::None;
            switch (motion_e.state)
            {
            case Button1Mask: { pressed_button = MouseButton::Left; break; }
            case Button2Mask: { pressed_button = MouseButton::Middle; break; }
            case Button3Mask: { pressed_button = MouseButton::Right; break; }
            default: break;
            }

            /*
            // Checking for window drag panel collision
            if (motion_e.state == Button1Mask && m_WindowStyle == WindowStyle::Modern)
            {
                Frame absolute_drag_frame = Frame(m_ModernWindowDragPanel->GetAbsolutePosition(), m_ModernWindowDragPanel->layer.frame.size);
        		absolute_drag_frame.AdjustToDpi(m_Dpi);

                if (absolute_drag_frame.DoesContain(click_location))
                {
                    PerformDragPanelCollision(m_Display, s_RegisteredWindows[m_RegisteredWindowIndex], GetAsboluteMouseCursorPos());
                    return;
                }
            }
            */

			auto mouse_moved_e = std::make_shared<MouseMovedEvent>(click_location, distance, pressed_button);
			auto hover_on_e = std::make_shared<MouseHoverOnEvent>(click_location, distance, pressed_button);
			auto hover_off_e = std::make_shared<MouseHoverOffEvent>(click_location, distance, pressed_button);

			m_SceneManager.DispatchEvent(mouse_moved_e);
			m_SceneManager.DispatchEvent(hover_on_e);
			m_SceneManager.DispatchEvent(hover_off_e);

			_mc_LinuxWindow_static_previous_mouse_position_ = click_location;
            break;
        }
        case KeyPress:
        {
            XKeyEvent& key_e = reinterpret_cast<XKeyEvent&>(event.xkey);

            bool shift_pressed = (bool)(key_e.state & ShiftMask);
            bool capslock = (bool)(key_e.state & LockMask);
            bool repeated = (bool)0;

            key_e.state &= ~ControlMask;
            KeyCode keycode = NativeToMcKeycode(XLookupKeysym(&key_e, 0));

            auto e = std::make_shared<KeyPressedEvent>(keycode, repeated, shift_pressed, capslock);
            m_SceneManager.DispatchEvent(e);
            XFlush(m_Display);
            break;
        }
        case KeyRelease:
        {
            XKeyEvent& key_e = reinterpret_cast<XKeyEvent&>(event.xkey);
            key_e.state &= ~ControlMask;
            KeyCode keycode = NativeToMcKeycode(XLookupKeysym(&key_e, 0));

            auto e = std::make_shared<KeyReleasedEvent>(keycode);
            m_SceneManager.DispatchEvent(e);
            break;
        }
        default: break;
        }
    }

    void LinuxWindow::Update()
    {
        XEvent event;
        XNextEvent(m_Display, &event);

        if (s_RegisteredWindows[m_RegisteredWindowIndex] == reinterpret_cast<XAnyEvent&>(event.xany).window)
        {
            ProcessEvents(&event);
        }
    }

    void LinuxWindow::ForceUpdate(bool clear_screen)
    {

    }

    void LinuxWindow::StartWindowLoop()
    {
        while (m_IsOpened)
        {
            XFlush(m_Display);
            usleep(16);
            Update();

            if (m_RegisteredWindowIndex != s_ActiveWindowIndex) continue;

            m_InternalTimer.reset();
			m_SceneManager.ProcessEvents(m_Dpi);
			auto EventProcessingTime = m_InternalTimer.elapsed();

			m_InternalTimer.reset();
			Graphics::Update(m_Background, m_SceneManager);
			auto GraphicsRenderingTime = m_InternalTimer.elapsed();

			auto e = std::make_shared<WindowUpdatedEvent>(nullptr, EventProcessingTime, GraphicsRenderingTime);
			m_SceneManager.DispatchEvent(e);
			m_SceneManager.ProcessEvents(m_Dpi);
        }

        Window window = s_RegisteredWindows[m_RegisteredWindowIndex];
        XDestroyWindow(m_Display, window);
        XCloseDisplay(m_Display);
    }

    void LinuxWindow::SetSize(uint32_t width, uint32_t height)
    {

    }

    void LinuxWindow::SetPos(uint32_t x, uint32_t y)
    {
        XMoveWindow(m_Display, s_RegisteredWindows[m_RegisteredWindowIndex], (int)x, (int)y);
    }

    void LinuxWindow::SetTitle(const char* title)
    {
        if (m_WindowStyle == WindowStyle::Modern)
            m_ModernWindowTitleLabel->Text = title;
    }

    void LinuxWindow::AddView(Ref<UIView> view)
    {
        view->srcwindow = this;
        m_SceneManager.AddView(view);
    }

    void LinuxWindow::RemoveView(Ref<UIView> view)
    {
        m_SceneManager.RemoveView(view);
    }

    void LinuxWindow::SetModernWindowButtonsColor(Color color)
    {
        if (m_ModernWindowCloseButton)
            m_ModernWindowCloseButton->layer.color = color;

        if (m_ModernWindowMaximizeButton)
            m_ModernWindowMaximizeButton->layer.color = color;

        if (m_ModernWindowMinimizeButton)
            m_ModernWindowMinimizeButton->layer.color = color;
    }

    void LinuxWindow::FocusView(Ref<UIView> view)
    {
        if (m_FocusedView == view.get()) return;

        // If a view was already focused, unfocus it and call event handlers
        if (m_FocusedView)
        {
            for (auto& callback : m_FocusedView->GetEventHandlers(EventType::FocusChanged))
            {
                auto event = FocusChangedEvent(false);
                callback(event, m_FocusedView);
            }
        }

        // Set the new focused view
        m_FocusedView = view.get();

        // If there is a new focused view, call its event handlers
        if (m_FocusedView)
        {
            for (auto& callback : m_FocusedView->GetEventHandlers(EventType::FocusChanged))
            {
                auto event = FocusChangedEvent(true);
                callback(event, m_FocusedView);
            }
        }
    }

    Ref<UIView> LinuxWindow::GetViewRef(UIView* raw_address)
    {
        return m_SceneManager.GetViewRef(raw_address);
    }

    Position LinuxWindow::GetMouseCursorPos()
    {
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;

        XQueryPointer(
            m_Display,
            s_RegisteredWindows[m_RegisteredWindowIndex],
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
        );

        return { (float)win_x_return, (float)win_y_return };
    }

    Position LinuxWindow::GetAsboluteMouseCursorPos()
    {
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;

        XQueryPointer(
            m_Display,
            s_RegisteredWindows[m_RegisteredWindowIndex],
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
        );

        return { (float)root_x_return, (float)root_y_return };
    }

    void LinuxWindow::RemoveAllViews()
    {
        for (auto& view : m_SceneManager.GetViewsList())
        {
            if (view.get() == m_ModernWindowDragPanel.get()) continue;
            if (view.get() == m_ModernWindowCloseButton.get()) continue;
            if (view.get() == m_ModernWindowMaximizeButton.get()) continue;
            if (view.get() == m_ModernWindowMinimizeButton.get()) continue;
            if (view.get() == m_ModernWindowTitleLabel.get()) continue;

            RemoveView(view);
        }
    }
}
