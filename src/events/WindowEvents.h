#pragma once
#include "Event.h"

namespace mc
{
	/// Event that is fired everytime the window changes size.
	class WindowEvent : public Event
	{
	public:
		/// Holds the handle to the window that fired the event.
		void* hwnd = 0;

		REGISTER_EVT_CATEGORY(EventCategoryWindow)

	protected:
		WindowEvent(void* hwnd)
			: hwnd(hwnd) {}
	};

	class WindowResizedEvent : public WindowEvent
	{
	public:
		WindowResizedEvent(void* hwnd, uint32_t width, uint32_t height)
			: WindowEvent(hwnd), width(width), height(height) {}

		/// Holds the width of the resized window.
		uint32_t width = 0;

		/// Holds the height of the resized window.
		uint32_t height = 0;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResized: " << std::to_string(width) << ", " << std::to_string(height);
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(WindowResized)
	};

	class WindowClosedEvent : public WindowEvent
	{
	public:
		WindowClosedEvent(void* hwnd) : WindowEvent(hwnd) {}
		REGISTER_EVT_CLASS_TYPE(WindowClosed)
	};

	class WindowGainedFocusEvent : public WindowEvent
	{
	public:
		WindowGainedFocusEvent(void* hwnd) : WindowEvent(hwnd) {}
		REGISTER_EVT_CLASS_TYPE(WindowGainedFocus)
	};

	class WindowLostFocusEvent : public WindowEvent
	{
	public:
		WindowLostFocusEvent(void* hwnd) : WindowEvent(hwnd) {}
		REGISTER_EVT_CLASS_TYPE(WindowLostFocus)
	};
}
