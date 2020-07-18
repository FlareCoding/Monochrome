#include "Win32Window.h"
#include <string>

#define MONOCHROME_72_WINDOW_CLASS_NAME "Monochrome72WindowClass"

LRESULT CALLBACK SetupWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace mc
{
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

std::shared_ptr<MCWindow> MCWindow::Create(uint32_t width, uint32_t height, const char* title)
{
	return std::shared_ptr<MCWindow>(new Win32Window(width, height, title));
}

Win32Window::Win32Window(uint32_t width, uint32_t height, const char* title)
	: m_Width(width), m_Height(height)
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
	wc.lpszMenuName = "";
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

	dwStyle = WS_OVERLAPPEDWINDOW;
	dwExStyle = WS_EX_OVERLAPPEDWINDOW;

	// Adjusting window rect to be exact size that we specified depending on the window style
	AdjustWindowRect(&window_rect, dwStyle, FALSE);

	// Creating window handle
	HWND hwnd = CreateWindowEx(
		dwExStyle, MONOCHROME_72_WINDOW_CLASS_NAME, title, dwStyle,
		window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top,
		NULL, NULL, NULL,
		this /* This parameter is crucial as it specifies the UIWindow pointer to be used in the WindowProc*/
	);

	if (hwnd == (HWND)nullptr)
	{
		MessageBoxA(0, "Failed to create window HWND", "UIWindow", 0);
		return;
	}

#pragma warning( push )
#pragma warning( disable : 6387 )
	// This disables the warning saying that 'hwnd' could be 0
	// because the error-checking is already being done

	// Showing and force-updating the window
	ShowWindow(hwnd, SW_SHOW);

#pragma warning( pop )
}

uint32_t Win32Window::GetWidth()
{
	return m_Width;
}

uint32_t Win32Window::GetHeight()
{
	return m_Height;
}
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

		/*const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		UIWindow* pWindow = reinterpret_cast<UIWindow*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			MessageBoxA(0, "Critical Error: Pointer to WindowData is null during WM_NCCREATE", "UIWindow", 0);
			return 0;
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MsgRedirectWindowProc));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);*/

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
