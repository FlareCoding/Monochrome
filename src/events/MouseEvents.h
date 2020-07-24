#pragma once
#include "Event.h"
#include "../UI/Layer.h"

namespace mc
{
	class UIView;

	enum class MouseButton
	{
		None = 0,
		Left,
		Right,
		Middle
	};

	/// Event that is fired on every mouse action (clicks, movement, etc.).
	class MouseEvent : public Event
	{
	public:
		/// Holds the mouse position in the window. 
		Point location;

		REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseEvent(Point location)
			: location(location) {}
	};

	/// Event that is fired on every mouse button action (press or release).
	class MouseButtonEvent : public MouseEvent
	{
	public:
		/// Holds the mouse button that was pressed or released. 
		MouseButton button;

		REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(Point location, MouseButton button)
			: MouseEvent(location), button(button) {}
	};

	/// Event that is fired on every mouse button press.
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(Point location, MouseButton button) : MouseButtonEvent(location, button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButton Pressed: " << std::to_string((int)button);
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseButtonPressed)
	};

	/// Event that is fired on every mouse button release.
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(Point location, MouseButton button) : MouseButtonEvent(location, button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButton Released: " << std::to_string((int)button);
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseButtonReleased)
	};

	using MouseButtonClickedEvent = MouseButtonReleasedEvent;

	/// Event that is fired when the mouse is moved.
	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(Point location, Point distance, MouseButton button) : MouseEvent(location), distance(distance), button(button) {}

		/// Holds the distance in pixels the mouse was moved.
		Point distance;

		/// Optionally holds a mouse button that is pressed in the process of movement.
		MouseButton button;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: " << std::to_string((int)location.x) << ", " << std::to_string((int)location.y);
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseMoved)
	};

	/// Event that is fired when the mouse is hovered over an element.
	class MouseHoverOnEvent : public MouseMovedEvent
	{
	public:
		MouseHoverOnEvent(Point location, Point distance, MouseButton button) : MouseMovedEvent(location, distance, button) {}
		REGISTER_EVT_CLASS_TYPE(MouseHoverOn)
	};

	/// Event that is fired when the mouse is hovered off an element.
	class MouseHoverOffEvent : public MouseMovedEvent
	{
	public:
		MouseHoverOffEvent(Point location, Point distance, MouseButton button) : MouseMovedEvent(location, distance, button) {}
		REGISTER_EVT_CLASS_TYPE(MouseHoverOff)
	};

	/// Event that is fired when the mouse wheel is scrolled.
	class MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(Point location, float distance) : MouseEvent(location), ScrollDistance(distance) {}

		/// Distance that the mouse wheel was scrolled.
		float ScrollDistance;

		REGISTER_EVT_CLASS_TYPE(MouseScrolled)
	};
}
