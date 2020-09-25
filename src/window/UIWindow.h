#pragma once
#include <core/Core.h>
#include <graphics/Color.h>
#include "WindowStyle.h"
#include "SceneManager.h"
#include <chrono>

#include "window/FileMenuBar.h"

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
		inline void* GetNativeHandle() const { return m_NativeHandle; }

		/// @returns Width of the window in pixels
		inline uint32_t GetWidth() const { return m_Width; }

		/// @returns Height of the window in pixels
		inline uint32_t GetHeight() const { return m_Height; }

		/// @returns Title of the window
		inline const char* const GetTitle() const { return m_Title; }

		/// @returns Window-specific DPI
		inline uint32_t GetDpi() const { return m_Dpi; }

		/// Sets the size of the window.
		virtual void SetSize(uint32_t width, uint32_t height) = 0;

		/// Sets the position of the window's top left corner.
		virtual void SetPos(uint32_t x, uint32_t y) = 0;

		/// Sets the title of the window.
		virtual void SetTitle(const char* title) = 0;

		/// Adds a UIView to the window's uiview hierarchy.
		virtual void AddView(Ref<UIView> view) = 0;

		/// Removes a UIView from the window's uiview hierarchy.
		virtual void RemoveView(Ref<UIView> view) = 0;

		/// Processes occured events and updates the screen buffer.
		virtual void Update() = 0;

		/// Forces events to be processed and graphics to be drawn.
		virtual void ForceUpdate(bool clear_screen = false) = 0;

		/// Starts the continous window loop which ends when the window closes
		virtual void StartWindowLoop() = 0;

		/// Sets a Menubar, which is going to be updated per frame
		virtual void SetMenuBar(const Ref<FileMenuBar> &menuBar) = 0;

		/// Returns whether or not the window was opened and still exists.
		/// If the window is manually hidden but hasn't been destroyed yet, returned result will still be true.
		inline bool const IsOpened() const { return m_IsOpened; }

		/// @returns Background color of the window.
		Color GetBackgroundColor() const { return m_Background; }

		/// Sets the window's background color.
		/// @param color Color to be used
		void SetBackgroundColor(Color color) { m_Background = color; }

		/// @returns Returns the pointer to the close ("x") button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetCloseButtonRef() const { return m_ModernWindowCloseButton; }

		/// @returns Returns the pointer to the maximize button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetMaximizeButtonRef() const { return m_ModernWindowMaximizeButton; }

		/// @returns Returns the pointer to the minimize button if the window was created with modern style, otherwise returns nullptr.
		Ref<UIButton> GetMinimizeButtonRef() const { return m_ModernWindowMinimizeButton; }

		/// @returns Returns the pointer to the window's title label if the window was created with modern style, otherwise returns nullptr.
		Ref<UILabel> GetWindowTitleLabelRef() const { return m_ModernWindowTitleLabel; }

		/// Sets the color of all modern style buttons in the top window bar.
		virtual void SetModernWindowButtonsColor(Color color) = 0;

		/// Sets focus onto a single view.
		/// If another view is already focused, that view will lose its focus.
		virtual void FocusView(Ref<UIView> view) = 0;

		/// @returns Returns the raw pointer to the currently focused UIView, but if no view is in focus, returns nullptr.
		UIView* GetFocusedView() { return m_FocusedView; };

		/// @returns Returns whether or not the following view is in focused state
		bool IsViewFocused(Ref<UIView>& view) { return view.get() == m_FocusedView; }

		/// @returns Returns whether or not the following view is in focused state
		bool IsViewFocused(UIView* view) { return view == m_FocusedView; }

		/// @returns Returns a shared_ptr to the uiview with the specified address.
		virtual Ref<UIView> GetViewRef(UIView* raw_address) = 0;

		/// @returns Returns the position of the mouse cursor relative to the top left corner of the window.
		virtual Position GetMouseCursorPos() = 0;

		/// @returns Returns the position of the mouse cursor relative to the top left corner of the monitor.
		virtual Position GetAsboluteMouseCursorPos() = 0;

		/// Removes all current views from the window's view hierarchy.
		virtual void RemoveAllViews() = 0;
		
		/// @returns Returns the position of the last ViewElement from the ViewList
		virtual const std::pair<float, float> GetLastViewPosition() = 0;

	protected:
		void* m_NativeHandle = nullptr;

		WindowStyle m_WindowStyle = WindowStyle::Default;
		uint32_t	m_Width = 0;
		uint32_t	m_Height = 0;
		const char* m_Title = 0;
		uint32_t	m_Dpi = 96;
		Color		m_Background = Color::gray;

		bool m_IsOpened = false;

		// Specifies whether the window is main or not.
		// This variable is used to determine when the application
		// should exit on WM_CLOSE message.
		bool m_IsMainWindow = false;

		virtual void Init() = 0;

		Ref<UIView>		m_ModernWindowDragPanel = nullptr;
		Ref<UIButton>	m_ModernWindowCloseButton = nullptr;
		Ref<UIButton>	m_ModernWindowMaximizeButton = nullptr;
		Ref<UIButton>	m_ModernWindowMinimizeButton = nullptr;
		Ref<UILabel>	m_ModernWindowTitleLabel = nullptr;

		virtual void SetupModernWindowViews() = 0;
		virtual void AdjustModernWindowViews() = 0;

		SceneManager m_SceneManager;
		UIView* m_FocusedView = nullptr;
	};
}
