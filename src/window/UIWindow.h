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
		/// @returns Raw pointer to the currently active window.
		static UIWindow* GetCurrentActiveWindow();

		/// Creates UIWindow instance and returns shared_ptr to it.
		/// @returns Pointer to the newly created window
		static Ref<UIWindow> Create(WindowStyle style = WindowStyle::Default, uint32_t width = 1270, uint32_t height = 920, const char* title = "Monochrome 7.2 Window");

		/// @returns Win32 handle to the native window
		inline HWND GetNativeHandle() const { return m_NativeHandle; }

		/// @returns Width of the window in pixels
		inline uint32_t GetWidth() const { return m_Width; }

		/// @returns Height of the window in pixels
		inline uint32_t GetHeight() const { return m_Height; }

		/// @returns Title of the window
		inline const char* const GetTitle() const { return m_Title; }

		/// @returns Window-specific DPI
		inline uint32_t GetDpi() const { return m_Dpi; }

		/// Sets the size of the window.
		void SetSize(uint32_t width, uint32_t height);

		/// Sets the position of the window's top left corner.
		void SetPos(uint32_t x, uint32_t y);

		/// Sets the title of the window.
		void SetTitle(const char* title);

		/// Adds a UIView to the window's uiview hierarchy.
		void AddView(Ref<UIView> view);

		/// Removes a UIView from the window's uiview hierarchy.
		void RemoveView(Ref<UIView> view);

		/// Processes occured events and updates the screen buffer.
		void Update();

		/// Forces events to be processed and graphics to be drawn.
		void ForceUpdate(bool clear_screen = false);

		/// Starts the continous window loop which ends when the window closes
		void StartWindowLoop();

		/// Returns whether or not the window was opened and still exists.
		/// If the window is manually hidden but hasn't been destroyed yet, returned result will still be true.
		inline bool const IsOpened() const { return m_IsOpened; }

		/// @returns Background color of the window.
		Color GetBackgroundColor() const		{ return m_Background; }

		/// Sets the window's background color.
		/// @param color Color to be used
		void SetBackgroundColor(Color color)	{ m_Background = color; }

		/// @returns Returns the pointer to the close ("x") button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetCloseButtonRef() const { return m_ModernWindowCloseButton; }

		/// @returns Returns the pointer to the maximize button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetMaximizeButtonRef() const { return m_ModernWindowMaximizeButton; }

		/// @returns Returns the pointer to the minimize button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetMinimizeButtonRef() const { return m_ModernWindowMinimizeButton; }

		/// @returns Returns the pointer to the window's title label if the window was created with modern style, otherwise returns nullptr.
		Ref<UILabel> GetWindowTitleLabelRef() const { return m_ModernWindowTitleLabel; }

		/// Sets the color of all modern style buttons in the top window bar.
		void SetModernWindowButtonsColor(Color color);

		/// Sets focus onto a single view.
		/// If another view is already focused, that view will lose its focus.
		void FocusView(Ref<UIView> view);

		/// @returns Returns the raw pointer to the currently focused UIView, but if no view is in focus, returns nullptr.
		UIView* GetFocusedView() { return m_FocusedView; };

		/// @returns Returns whether or not the following view is in focused state
		bool IsViewFocused(Ref<UIView>& view) { return view.get() == m_FocusedView; }

		/// @returns Returns whether or not the following view is in focused state
		bool IsViewFocused(UIView* view) { return view == m_FocusedView; }

		/// Returns a shared_ptr to the uiview with the specified address.
		Ref<UIView> GetViewRef(UIView* raw_address);

		/// Returns the position of the mouse cursor relative to the top left corner of the window.
		Position GetMouseCursorPos();

		/// Returns the position of the mouse cursor relative to the top left corner of the monitor.
		Position GetAsboluteMouseCursorPos();

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
		bool m_IsMainWindow = false;

		void Init();

		Ref<UIView>		m_ModernWindowDragPanel			= nullptr;
		Ref<UIButton>	m_ModernWindowCloseButton		= nullptr;
		Ref<UIButton>	m_ModernWindowMaximizeButton	= nullptr;
		Ref<UIButton>	m_ModernWindowMinimizeButton	= nullptr;
		Ref<UILabel>	m_ModernWindowTitleLabel		= nullptr;

		void SetupModernWindowViews();
		void AdjustModernWindowViews();

	private:
		SceneManager m_SceneManager;

		UIView* m_FocusedView = nullptr;

	public:
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
