#include "WindowsWindow.h"
#include <graphics/Graphics.h>
#include <ui/UIElements.h>
#include <events/Events.h>

#include <locale>
#include <codecvt>

#define GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))

std::wstring ConvertStringToWstring(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.length(), NULL, 0);
	std::wstring wstrTo;
	if (num_chars)
	{
		wstrTo.resize(num_chars);
		if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.length(), &wstrTo[0], num_chars))
		{
			return wstrTo;
		}
	}
	return std::wstring();
}

namespace mc
{
	typedef BOOL (__stdcall *SetProcessDpiAwarenessContextFn)(DPI_AWARENESS_CONTEXT);

	static WindowsWindow* s_CurrentActiveWindowInstance = nullptr;

	static POINT _mc_WindowsWindow_static_previous_mouse_position_;

	static bool s_MainWindowClosed = false;

#define MONOCHROME_70_WINDOW_CLASSNAME L"Monochrome70Window"
	static uint32_t WindowInstancesCreated = 0;

	LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MsgRedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static bool ModernWindowCloseButton_OnMouseClick(Event& event, UIView* sender)
	{
		if (((MouseButtonEvent&)event).button == MouseButton::Left)
			SendMessage(reinterpret_cast<HWND>(sender->srcwindow->GetNativeHandle()), WM_CLOSE, 0, 0);

		return EVENT_HANDLED;
	}

	static bool ModernWindowMaximizeButton_OnMouseClick(Event& event, UIView* sender)
	{
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(reinterpret_cast<HWND>(sender->srcwindow->GetNativeHandle()), &wndpl);

		if (((MouseButtonEvent&)event).button == MouseButton::Left)
		{
			// The window should be maximized if it's not already and
			// should return to normal state if it was already maximized.
			if (wndpl.showCmd == SW_MAXIMIZE)
				ShowWindow(reinterpret_cast<HWND>(sender->srcwindow->GetNativeHandle()), SW_NORMAL);
			else
				ShowWindow(reinterpret_cast<HWND>(sender->srcwindow->GetNativeHandle()), SW_MAXIMIZE);
		}

		return EVENT_HANDLED;
	}

	static bool ModernWindowMinimizeButton_OnMouseClick(Event& event, UIView* sender)
	{
		if (((MouseButtonEvent&)event).button == MouseButton::Left)
			ShowWindow(reinterpret_cast<HWND>(sender->srcwindow->GetNativeHandle()), SW_MINIMIZE);

		return EVENT_HANDLED;
	}

	UIWindow* UIWindow::GetCurrentActiveWindow()
	{
		return s_CurrentActiveWindowInstance;
	}

	Ref<UIWindow> UIWindow::Create(WindowStyle style, uint32_t width, uint32_t height, const char* title)
	{
		return Ref<UIWindow>(new WindowsWindow(style, width, height, title));
	}

	WindowsWindow::WindowsWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title)
	{
		m_WindowStyle = style;
		m_Width = width;
		m_Height = height;
		m_Title = title;
		Init();
	}

	void WindowsWindow::Init()
	{
#pragma warning( push )
#pragma warning( disable : 6387 )
		SetProcessDpiAwarenessContextFn DpiAwarenessProc = (SetProcessDpiAwarenessContextFn)GetProcAddress(LoadLibraryA("User32.dll"), "SetProcessDpiAwarenessContext");

		// Set window DPI-Awareness mode to be aware of high dpi displays
		if (DpiAwarenessProc)
			DpiAwarenessProc(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
#pragma warning( pop )

		WindowInstancesCreated++;
		std::wstring WindowClassName = std::wstring(MONOCHROME_70_WINDOW_CLASSNAME + std::to_wstring(WindowInstancesCreated));

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hInstance = NULL;
		wc.lpszClassName = WindowClassName.c_str();
		wc.lpszMenuName = L"";
		wc.lpfnWndProc = SetupWindowProc;
		wc.style = NULL;

		// Registering Win32 API Class
		if (!RegisterClassEx(&wc))
		{
			MessageBoxA(0, "Failed to register Win32 window class", "WindowsWindow", 0);
			return;
		}

		// Creating window rect
		RECT window_rect;
		window_rect.left = 60;
		window_rect.top = 60;
		window_rect.right = window_rect.left + m_Width;
		window_rect.bottom = window_rect.top + m_Height;

		DWORD dwStyle = 0;
		DWORD dwExStyle = 0;

		if (m_WindowStyle == WindowStyle::Classic)
		{
			dwStyle = WS_OVERLAPPEDWINDOW;
			dwExStyle = WS_EX_OVERLAPPEDWINDOW;
		}
		else if (m_WindowStyle == WindowStyle::Modern)
		{
			dwStyle = WS_POPUP;
			dwExStyle = WS_EX_APPWINDOW;
		}

		// Adjusting window rect to be exact size that we specified depending on the window style
		AdjustWindowRect(&window_rect, dwStyle, FALSE);

		// Creating window handle
		HWND hwnd = CreateWindowEx(
			dwExStyle, WindowClassName.c_str(), ConvertStringToWstring(m_Title).c_str(), dwStyle,
			window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
			NULL, NULL, NULL,
			this /* This parameter is crucial as it specifies the WindowsWindow pointer to be used in the WindowProc*/
		);

		if (hwnd == (HWND)nullptr)
		{
			MessageBoxA(0, "Failed to create window HWND", "WindowsWindow", 0);
			return;
		}

		m_NativeHandle = hwnd;

#pragma warning( push )
#pragma warning( disable : 6387 )
		// This disables the warning saying that 'hwnd' could be 0
		// because the error-checking is already being done

		// Showing and force-updating the window
		ShowWindow(hwnd, SW_SHOW);

		m_IsOpened = true;
		UpdateWindow(hwnd);
#pragma warning( pop )

		// Calculating window DPI for current monitor
		m_Dpi = GetDpiForWindow(reinterpret_cast<HWND>(m_NativeHandle));

		// If the window is of modern style
		if (m_WindowStyle == WindowStyle::Modern)
		{
			// Setup modern window ui elements
			SetupModernWindowViews();
		}

		if (WindowInstancesCreated == 1)
			m_IsMainWindow = true;
	}

	void WindowsWindow::SetupModernWindowViews()
	{
		m_ModernWindowDragPanel = MakeRef<UIView>();
		m_ModernWindowDragPanel->layer.frame = Frame(0, 0, (float)m_Width - 160, 60);
		m_ModernWindowDragPanel->layer.color = Color::transparent;
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

	void WindowsWindow::AdjustModernWindowViews()
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

	LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		{
			// Retrieves WindowsWindow pointer from the initial createstruct data and assigns it to the GWLP_USERDATA
			// so it can be accessed later from the main running WindowProc.
			// It also sets MsgRedirectWindowProc as the new primary window procedure.

			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(pCreate->lpCreateParams);
			if (pWindow == nullptr)
			{
				MessageBoxA(0, "Critical Error: Pointer to WindowData is null during WM_NCCREATE", "WindowsWindow", 0);
				return 0;
			}
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MsgRedirectWindowProc));
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK MsgRedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Retrieves WindowsWindow pointer from the GWL_USERDATA and calls the WindowsWindow's own window procedure.
		WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			Graphics::Initialize(hwnd);
			Graphics::SetActiveTarget(hwnd);
			s_CurrentActiveWindowInstance = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			break;
		}
		case WM_CLOSE:
		{
			// Retrieveing WindowsWindow pointer
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			auto e = std::make_shared<WindowClosedEvent>(hwnd);
			m_SceneManager.DispatchEvent(e);
			m_SceneManager.ProcessEvents(m_Dpi);

			// Setting the opened flag to false
			pWindow->m_IsOpened = false;

			// Graphics should shutdown when the main window closes
			if (m_IsMainWindow)
			{
				Graphics::Shutdown();
				s_MainWindowClosed = true;
			}

			// Destroying the win32 window object
			DestroyWindow(hwnd);

			break;
		}
		case WM_ACTIVATE:
		{
			// Retrieveing WindowsWindow pointer
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			unsigned short activation_flag = LOWORD(wParam);
			if (activation_flag == WA_ACTIVE || activation_flag == WA_CLICKACTIVE)
			{
				Graphics::SetActiveTarget(hwnd);
				s_CurrentActiveWindowInstance = pWindow;

				auto e = std::make_shared<WindowGainedFocusEvent>(s_CurrentActiveWindowInstance->GetNativeHandle());
				m_SceneManager.DispatchEvent(e);
				m_SceneManager.ProcessEvents(m_Dpi);
			}
			else
			{
				auto e = std::make_shared<WindowLostFocusEvent>(s_CurrentActiveWindowInstance->GetNativeHandle());
				m_SceneManager.DispatchEvent(e);
				m_SceneManager.ProcessEvents(m_Dpi);

				s_CurrentActiveWindowInstance = nullptr;
			}

			break;
		}
		case WM_SIZE:
		{
			Graphics::ResizeRenderTarget(hwnd);

			INT nWidth = LOWORD(lParam);
			INT nHeight = HIWORD(lParam);

			// Retrieveing WindowsWindow pointer
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			pWindow->m_Width = nWidth;
			pWindow->m_Height = nHeight;

			if (pWindow->m_WindowStyle == WindowStyle::Modern)
				pWindow->AdjustModernWindowViews();

			if (pWindow->m_ModernWindowMaximizeButton)
			{
				WINDOWPLACEMENT wndpl;
				wndpl.length = sizeof(WINDOWPLACEMENT);
				GetWindowPlacement(reinterpret_cast<HWND>(pWindow->m_NativeHandle), &wndpl);

				if (wndpl.showCmd == SW_MAXIMIZE)
					pWindow->m_ModernWindowMaximizeButton->Label->WidestringText = L"🗗";
				else
					pWindow->m_ModernWindowMaximizeButton->Label->WidestringText = L"⬜";
			}

			auto e = std::make_shared<WindowResizedEvent>(hwnd, (uint32_t)nWidth, (uint32_t)nHeight);
			m_SceneManager.DispatchEvent(e);
			pWindow->ForceUpdate(true);
			break;
		}
		case WM_MOVE:
		{
			// Retrieveing WindowsWindow pointer
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			pWindow->ForceUpdate();
			break;
		}
		case WM_NCHITTEST:
		{
			// Call the default window procedure for default handling.
			const LRESULT result = DefWindowProc(hwnd, uMsg, wParam, lParam);

			// Retrieveing WindowsWindow pointer
			WindowsWindow* pWindow = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			if (pWindow->m_WindowStyle == WindowStyle::Modern)
			{
				// Get the location of the mouse click, which is packed into lParam.
				POINT pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				const auto CheckForResizingConditions = [this, pWindow](POINT pt, LONG BorderWidth, LRESULT& result) -> bool {
					RECT ClientArea;
					GetClientRect(reinterpret_cast<HWND>(pWindow->GetNativeHandle()), &ClientArea);
					ScreenToClient(reinterpret_cast<HWND>(pWindow->GetNativeHandle()), &pt);

					/*top-left, top and top-right*/
					if (pt.y < BorderWidth)
					{
						if (pt.x < BorderWidth)
						{
							result = HTTOPLEFT;
							return true;
						}
						else if (pt.x > (ClientArea.right - BorderWidth))
						{
							result = HTTOPRIGHT;
							return true;
						}
						result = HTTOP;
						return true;
					}
					/*bottom-left, bottom and bottom-right */
					if (pt.y > (ClientArea.bottom - BorderWidth))
					{
						if (pt.x < BorderWidth)
						{
							result = HTBOTTOMLEFT;
							return true;
						}
						else if (pt.x > (ClientArea.right - BorderWidth))
						{
							result = HTBOTTOMRIGHT;
							return true;
						}

						result = HTBOTTOM;
						return true;
					}
					if (pt.x < BorderWidth)
					{
						result = HTLEFT;
						return true;
					}
					if (pt.x > (ClientArea.right - BorderWidth))
					{
						result = HTRIGHT;
						return true;
					}

					return false;
				};

				LRESULT ResizingConditionsResult = 0;
				static LONG BorderWidth = 5;
				if (CheckForResizingConditions(pt, BorderWidth, ResizingConditionsResult))
					return ResizingConditionsResult;

				RECT rcDraggable = { 0, 0, (LONG)pWindow->m_ModernWindowDragPanel->layer.frame.size.width, (LONG)pWindow->m_ModernWindowDragPanel->layer.frame.size.height };

				MapWindowPoints(hwnd, NULL, reinterpret_cast<POINT*>(&rcDraggable), (sizeof(RECT) / sizeof(POINT)));
				if ((result == HTCLIENT) && (PtInRect(&rcDraggable, pt)))
				{
					return HTCAPTION;
				}
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonPressedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Left);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonPressedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Right);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_MBUTTONDOWN:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonPressedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Middle);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_LBUTTONUP:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Left);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_RBUTTONUP:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Right);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_MBUTTONUP:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Middle);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_MOUSEMOVE:
		{
			auto click_absolute_point = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };

			auto click_location = Point{ (float)click_absolute_point.x, (float)click_absolute_point.y };
			auto distance = Point{
				click_location.x - (float)_mc_WindowsWindow_static_previous_mouse_position_.x,
				click_location.y - (float)_mc_WindowsWindow_static_previous_mouse_position_.y
			};

			MouseButton pressed_button = MouseButton::None;
			if (wParam == MK_LBUTTON)		pressed_button = MouseButton::Left;
			else if (wParam == MK_RBUTTON)	pressed_button = MouseButton::Right;
			else if (wParam == MK_MBUTTON)	pressed_button = MouseButton::Middle;

			auto mouse_moved_e = std::make_shared<MouseMovedEvent>(click_location, distance, pressed_button);
			auto hover_on_e = std::make_shared<MouseHoverOnEvent>(click_location, distance, pressed_button);
			auto hover_off_e = std::make_shared<MouseHoverOffEvent>(click_location, distance, pressed_button);

			m_SceneManager.DispatchEvent(mouse_moved_e);
			m_SceneManager.DispatchEvent(hover_on_e);
			m_SceneManager.DispatchEvent(hover_off_e);

			_mc_WindowsWindow_static_previous_mouse_position_ = click_absolute_point;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			float distance = (float)GET_WHEEL_DELTA_WPARAM(wParam);

			auto e = std::make_shared<MouseScrolledEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, distance);
			m_SceneManager.DispatchEvent(e);

			break;
		}
		case WM_KEYDOWN:
		{
			int vk_keycode = (int)wParam;
			bool shift_pressed = (bool)(GetKeyState(VK_SHIFT) & 0x8000);
			bool capslock = (bool)(GetKeyState(VK_CAPITAL) & 0x0001);
			bool repeated = (bool)(HIWORD(lParam) & KF_REPEAT);
			KeyCode keycode = NativeToMcKeycode(vk_keycode);

			auto e = std::make_shared<KeyPressedEvent>(keycode, repeated, shift_pressed, capslock);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_KEYUP:
		{
			int vk_keycode = (int)wParam;
			KeyCode keycode = NativeToMcKeycode(vk_keycode);

			auto e = std::make_shared<KeyReleasedEvent>(keycode);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_SETCURSOR:
		{
			UICursor::ActivateCursor(UICursor::GetActiveCursor());
			break;
		}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void WindowsWindow::Update()
	{
		// Process all messages and events
		while (PeekMessage(&msg, reinterpret_cast<HWND>(m_NativeHandle), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If message is WM_NULL, check if window is destroyed and does not exist anymore
		if (msg.message == WM_NULL)
		{
			if (!IsWindow(reinterpret_cast<HWND>(m_NativeHandle)))
			{
				m_NativeHandle = NULL;
				return;
			}
		}
	}

	void WindowsWindow::ForceUpdate(bool clear_screen)
	{
		WindowsWindow* TrueActiveWindowInstance = s_CurrentActiveWindowInstance;
		s_CurrentActiveWindowInstance = this;
		Graphics::SetActiveTarget(reinterpret_cast<HWND>(m_NativeHandle));

		Update();
		m_SceneManager.ProcessEvents(m_Dpi);
		Graphics::Update(m_Background, m_SceneManager, clear_screen);

		if (TrueActiveWindowInstance)
		{
			s_CurrentActiveWindowInstance = TrueActiveWindowInstance;
			Graphics::SetActiveTarget(reinterpret_cast<HWND>(s_CurrentActiveWindowInstance->GetNativeHandle()));

			s_CurrentActiveWindowInstance->Update();
			s_CurrentActiveWindowInstance->m_SceneManager.ProcessEvents(m_Dpi);
			Graphics::Update(m_Background, s_CurrentActiveWindowInstance->m_SceneManager, clear_screen);
		}
	}

	void WindowsWindow::StartWindowLoop()
	{
		while (IsOpened())
		{
			Sleep(16); // 16 miliseconds is the most optimal sleep amount (1 frame per 16 ms.)
			Update();

			if (s_MainWindowClosed) return;

			if (GetActiveWindow() != reinterpret_cast<HWND>(m_NativeHandle)) continue;

			s_CurrentActiveWindowInstance = this;

			m_InternalTimer.reset();
			m_SceneManager.ProcessEvents(m_Dpi);
			auto EventProcessingTime = m_InternalTimer.elapsed();

			m_InternalTimer.reset();
			Graphics::Update(m_Background, m_SceneManager);
			auto GraphicsRenderingTime = m_InternalTimer.elapsed();

			auto e = std::make_shared<WindowUpdatedEvent>(reinterpret_cast<HWND>(m_NativeHandle), EventProcessingTime, GraphicsRenderingTime);
			m_SceneManager.DispatchEvent(e);
			m_SceneManager.ProcessEvents(m_Dpi);
		}
	}

	void WindowsWindow::SetSize(uint32_t width, uint32_t height)
	{
		// Get window's current position and size
		RECT rect;
		GetWindowRect(reinterpret_cast<HWND>(m_NativeHandle), &rect);

		// Set new window size
		SetWindowPos(reinterpret_cast<HWND>(m_NativeHandle), NULL, rect.left, rect.top, width, height, 0);

		// Adjust modern window widget's position and size
		if (m_WindowStyle == WindowStyle::Modern)
			AdjustModernWindowViews();
	}

	void WindowsWindow::SetPos(uint32_t x, uint32_t y)
	{
		// Get window's current position and size
		RECT rect;
		GetWindowRect(reinterpret_cast<HWND>(m_NativeHandle), &rect);

		int width = (int)(rect.right - rect.left);
		int height = (int)(rect.bottom - rect.top);

		// Set new window position
		SetWindowPos(reinterpret_cast<HWND>(m_NativeHandle), NULL, x, y, width, height, 0);
	}

	void WindowsWindow::SetTitle(const char* title)
	{
		SetWindowTextA(reinterpret_cast<HWND>(m_NativeHandle), title);

		if (m_WindowStyle == WindowStyle::Modern)
			m_ModernWindowTitleLabel->Text = title;
	}

	void WindowsWindow::AddView(Ref<UIView> view)
	{
		view->srcwindow = this;
		m_SceneManager.AddView(view);
	}

	void WindowsWindow::RemoveView(Ref<UIView> view)
	{
		m_SceneManager.RemoveView(view);
	}

	void WindowsWindow::SetModernWindowButtonsColor(Color color)
	{
		if (m_ModernWindowCloseButton)
			m_ModernWindowCloseButton->layer.color = color;

		if (m_ModernWindowMaximizeButton)
			m_ModernWindowMaximizeButton->layer.color = color;

		if (m_ModernWindowMinimizeButton)
			m_ModernWindowMinimizeButton->layer.color = color;
	}

	void WindowsWindow::FocusView(Ref<UIView> view)
	{
		if (m_FocusedView == view.get()) return;

		// If a view was already focused, unfocus it and call event handlers
		if (m_FocusedView)
		{
			for (auto& callback : m_FocusedView->GetEventHandlers(EventType::FocusChanged))
			{
				auto& event = FocusChangedEvent(false);
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
				auto& event = FocusChangedEvent(true);
				callback(event, m_FocusedView);
			}
		}
	}

	Ref<UIView> WindowsWindow::GetViewRef(UIView* raw_address)
	{
		return m_SceneManager.GetViewRef(raw_address);
	}

	Position WindowsWindow::GetMouseCursorPos()
	{
		Position position = { -1, -1 };

		POINT p;
		if (!GetCursorPos(&p))
			return position;

		if (!ScreenToClient(reinterpret_cast<HWND>(reinterpret_cast<HWND>(m_NativeHandle)), &p))
			return position;

		position = { (float)p.x, (float)p.y };
		return position;
	}

	Position WindowsWindow::GetAsboluteMouseCursorPos()
	{
		Position position = { -1, -1 };

		POINT p;
		if (!GetCursorPos(&p))
			return position;

		position = { (float)p.x, (float)p.y };
		return position;
	}

	void WindowsWindow::RemoveAllViews()
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
