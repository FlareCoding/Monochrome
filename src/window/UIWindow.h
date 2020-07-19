#pragma once
#include <core/Core.h>
#include "WindowStyle.h"
#include <graphics/Color.h>
#include "SceneManager.h"
#include <Windows.h>

namespace mc
{
	class UILabel;
	class UIButton;

	class UIWindow
	{
	public:
		static UIWindow* GetCurrentActiveWindow();

		/// <summary> Creates UIWindow instance and returns shared_ptr to it </summary>
		static Ref<UIWindow> Create(WindowStyle style = WindowStyle::Default, uint32_t width = 1270, uint32_t height = 920, const char* title = "Monochrome 6.0 Window");

		// Returns Win32 handle to the native window
		inline HWND GetNativeHandle() const { return m_NativeHandle; }

		// Returns width of the window in pixels
		inline uint32_t GetWidth() const { return m_Width; }

		// Returns height of the window in pixels
		inline uint32_t GetHeight() const { return m_Height; }

		// Returns window title
		inline const char* const GetTitle() const { return m_Title; }

		// Returns window-specific DPI
		inline uint32_t GetDpi() const { return m_Dpi; }

		// Adds a UIView to the window's uiview hierarchy
		void AddView(Ref<UIView> view);

		// Processed occured events and updates the screen buffer
		void Update();

		// Starts the continous window loop which ends when the window closes
		void StartWindowLoop();

		// Returns whether or not the window was opened and still exists.
		// If the window is manually hidden but hasn't been destroyed yet, returned result will still be true.
		inline bool const IsOpened() const { return m_IsOpened; }

		Color GetBackgroundColor() const		{ return m_Background; }
		void SetBackgroundColor(Color color)	{ m_Background = color; }

		Ref<UIButton> GetCloseButtonRef() const { return m_ModernWindowCloseButton; }
		Ref<UILabel> GetWindowTitleLabelRef() const { return m_ModernWindowTitleLabel; }

	private:
		UIWindow(WindowStyle style, uint32_t width, uint32_t height, const char* title);
		HWND m_NativeHandle = {};
		MSG msg = {};

	private:
		WindowStyle m_WindowStyle = WindowStyle::Default;
		uint32_t	m_Width		= 0;
		uint32_t	m_Height	= 0;
		const char* m_Title		= 0;
		uint32_t	m_Dpi		= 0;
		Color		m_Background = Color::gray;

		bool m_IsOpened = false;

		// Specifies whether the window is main or not.
		// This variable is used to determine when the application
		// should exit on WM_CLOSE message.
		bool m_IsMainWindow = true;

		void Init();

		Ref<UIView>		m_ModernWindowDragPanel		= nullptr;
		Ref<UIButton>	m_ModernWindowCloseButton	= nullptr;
		Ref<UILabel>	m_ModernWindowTitleLabel	= nullptr;

		void SetupModernWindowViews();

	private:
		SceneManager m_SceneManager;

	public:
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
