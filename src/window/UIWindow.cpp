#include "UIWindow.h"
#include <graphics/Graphics.h>
#include <ui/UIElements.h>
#include <events/Events.h>

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
#define MONOCHROME_72_WINDOW_CLASS_NAME L"Monochrome60WindowClass"

	static UIWindow* s_CurrentActiveWindowInstance = nullptr;

	static POINT _mc_uiwindow_static_previous_mouse_position_;

	LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MsgRedirectWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	static bool ModernWindowButton_OnMouseClick(Event& event, UIView* sender)
	{
		if (((MouseButtonEvent&)event).button == MouseButton::Left)
		{
			SendMessage(sender->srcwindow->GetNativeHandle(), WM_CLOSE, 0, 0);
		}

		return EVENT_HANDLED;
	}

	UIWindow* UIWindow::GetCurrentActiveWindow()
	{
		return s_CurrentActiveWindowInstance;
	}

	Ref<UIWindow> UIWindow::Create(WindowStyle style, uint32_t width, uint32_t height, const char* title)
	{
		return Ref<UIWindow>(new UIWindow(style, width, height, title));
	}

	UIWindow::UIWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title)
		: m_WindowStyle(style), m_Width(width), m_Height(height), m_Title(title)
	{
		Init();
	}

	void UIWindow::Init()
	{
		// Set window DPI-Awareness mode to be aware of high dpi displays
		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hInstance = NULL;
		wc.lpszClassName = MONOCHROME_72_WINDOW_CLASS_NAME;
		wc.lpszMenuName = L"";
		wc.lpfnWndProc = SetupWindowProc;
		wc.style = NULL;

		// Registering Win32 API Class
		if (!RegisterClassEx(&wc))
		{
			MessageBoxA(0, "Failed to register Win32 window class", "UIWindow", 0);
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
			dwExStyle, MONOCHROME_72_WINDOW_CLASS_NAME, ConvertStringToWstring(m_Title).c_str(), dwStyle,
			window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
			NULL, NULL, NULL,
			this /* This parameter is crucial as it specifies the UIWindow pointer to be used in the WindowProc*/
		);

		if (hwnd == (HWND)nullptr)
		{
			MessageBoxA(0, "Failed to create window HWND", "UIWindow", 0);
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
		m_Dpi = GetDpiForWindow(m_NativeHandle);

		// If the window is of modern style
		if (m_WindowStyle == WindowStyle::Modern)
		{
			// Setup modern window ui elements
			SetupModernWindowViews();
		}
	}

	void UIWindow::SetupModernWindowViews()
	{
		m_ModernWindowDragPanel = MakeRef<UIView>();
		m_ModernWindowDragPanel->layer.frame = Frame(0, 0, (float)m_Width - 60, 60);
		m_ModernWindowDragPanel->layer.color = Color::transparent;
		m_ModernWindowDragPanel->SetZIndex(10000);
		AddView(m_ModernWindowDragPanel);

		m_ModernWindowCloseButton = MakeRef<UIButton>();
		m_ModernWindowCloseButton->layer.frame = Frame((float)m_Width - 46, 0, 46, 36);
		m_ModernWindowCloseButton->CornerRadius = 0;
		m_ModernWindowCloseButton->Label->Text = "X";
		m_ModernWindowCloseButton->HoverOnColor = Color::red;
		m_ModernWindowCloseButton->SetZIndex(10000);
		m_ModernWindowCloseButton->AddEventHandler<EventType::MouseButtonReleased>(ModernWindowButton_OnMouseClick);
		AddView(CastToUiView(m_ModernWindowCloseButton));

		m_ModernWindowTitleLabel = MakeRef<UILabel>();
		m_ModernWindowTitleLabel->layer.frame = Frame(10, 8, 300, 26);
		m_ModernWindowTitleLabel->layer.color = Color::black;
		m_ModernWindowTitleLabel->Text = m_Title;
		m_ModernWindowTitleLabel->Properties.Allignment = TextAlignment::LEADING;
		m_ModernWindowTitleLabel->color = Color::white;
		m_ModernWindowTitleLabel->SetZIndex(10000);
		AddView(CastToUiView(m_ModernWindowTitleLabel));
	}

	LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		{
			// Retrieves UIWindow pointer from the initial createstruct data and assigns it to the GWLP_USERDATA
			// so it can be accessed later from the main running WindowProc.
			// It also sets MsgRedirectWindowProc as the new primary window procedure.

			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			UIWindow* pWindow = reinterpret_cast<UIWindow*>(pCreate->lpCreateParams);
			if (pWindow == nullptr)
			{
				MessageBoxA(0, "Critical Error: Pointer to WindowData is null during WM_NCCREATE", "UIWindow", 0);
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
		// Retrieves UIWindow pointer from the GWL_USERDATA and calls the UIWindow's own window procedure.
		UIWindow* pWindow = reinterpret_cast<UIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT UIWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			Graphics::Initialize(hwnd);
			s_CurrentActiveWindowInstance = reinterpret_cast<UIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));;

			break;
		}
		case WM_CLOSE:
		{
			// Retrieveing UIWindow pointer
			UIWindow* pWindow = reinterpret_cast<UIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			// Setting the opened flag to false
			pWindow->m_IsOpened = false;

			// Destroying the win32 window object
			DestroyWindow(hwnd);

			break;
		}
		case WM_SIZE:
		{
			Graphics::ResizeRenderTarget(hwnd);

			INT nWidth = LOWORD(lParam);
			INT nHeight = HIWORD(lParam);

			// Retrieveing UIWindow pointer
			UIWindow* pWindow = reinterpret_cast<UIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			pWindow->m_Width = nWidth;
			pWindow->m_Height = nHeight;

			break;
		}
		case WM_NCHITTEST:
		{
			// Call the default window procedure for default handling.
			const LRESULT result = DefWindowProc(hwnd, uMsg, wParam, lParam);

			// Retrieveing UIWindow pointer
			UIWindow* pWindow = reinterpret_cast<UIWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			if (pWindow->m_WindowStyle == WindowStyle::Modern)
			{
				// Get the location of the mouse click, which is packed into lParam.
				POINT pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

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
		case  WM_MBUTTONDOWN:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonPressedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Middle);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case  WM_LBUTTONUP:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Left);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case  WM_RBUTTONUP:
		{
			auto clickpos = POINT{ (LONG)((float)GET_X_LPARAM(lParam) * (float)m_Dpi / 96.0f), (LONG)((float)GET_Y_LPARAM(lParam) * (float)m_Dpi / 96.0f) };
			auto e = std::make_shared<MouseButtonReleasedEvent>(Point{ (float)clickpos.x, (float)clickpos.y }, MouseButton::Right);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case  WM_MBUTTONUP:
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
				click_location.x - (float)_mc_uiwindow_static_previous_mouse_position_.x,
				click_location.y - (float)_mc_uiwindow_static_previous_mouse_position_.y
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

			_mc_uiwindow_static_previous_mouse_position_ = click_absolute_point;
			break;
		}
		case WM_KEYDOWN:
		{
			int vk_keycode = (int)wParam;
			bool shift_pressed = (bool)(GetKeyState(VK_SHIFT) & 0x8000);
			bool capslock = (bool)(GetKeyState(VK_CAPITAL) & 0x0001);
			bool repeated = (bool)(HIWORD(lParam) & KF_REPEAT);
			KeyCode keycode = VkToMcKeycode(vk_keycode);

			auto e = std::make_shared<KeyPressedEvent>(keycode, repeated, shift_pressed, capslock);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		case WM_KEYUP:
		{
			int vk_keycode = (int)wParam;
			KeyCode keycode = VkToMcKeycode(vk_keycode);

			auto e = std::make_shared<KeyReleasedEvent>(keycode);
			m_SceneManager.DispatchEvent(e);
			break;
		}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void UIWindow::Update()
	{
		// Process all messages and events
		while (PeekMessage(&msg, m_NativeHandle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If message is WM_NULL, check if window is destroyed and does not exist anymore
		if (msg.message == WM_NULL)
		{
			if (!IsWindow(m_NativeHandle))
			{
				m_NativeHandle = NULL;
				return;
			}
		}
	}

	void UIWindow::StartWindowLoop()
	{
		while (IsOpened())
		{
			Sleep(20);
			Update();

			m_SceneManager.ProcessEvents(m_Dpi);
			Graphics::Update(m_Background, m_SceneManager);
		}
	}

	void UIWindow::AddView(Ref<UIView> view)
	{
		view->srcwindow = this;
		m_SceneManager.AddView(view);
	}
}
