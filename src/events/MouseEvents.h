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

	class MouseEvent : public Event
	{
	public:
		Point location;

		REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput)

	protected:
		MouseEvent(Point location)
			: location(location) {}
	};

	class MouseButtonEvent : public MouseEvent
	{
	public:
		MouseButton button;

		REGISTER_EVT_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

	protected:
		MouseButtonEvent(Point location, MouseButton button)
			: MouseEvent(location), button(button) {}
	};

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

	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(Point location, Point distance, MouseButton button) : MouseEvent(location), distance(distance), button(button) {}

		Point distance;
		MouseButton button;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: " << std::to_string((int)location.x) << ", " << std::to_string((int)location.y);
			return ss.str();
		}

		REGISTER_EVT_CLASS_TYPE(MouseMoved)
	};

	class MouseHoverOnEvent : public MouseMovedEvent
	{
	public:
		MouseHoverOnEvent(Point location, Point distance, MouseButton button) : MouseMovedEvent(location, distance, button) {}
		REGISTER_EVT_CLASS_TYPE(MouseHoverOn)
	};

	class MouseHoverOffEvent : public MouseMovedEvent
	{
	public:
		MouseHoverOffEvent(Point location, Point distance, MouseButton button) : MouseMovedEvent(location, distance, button) {}
		REGISTER_EVT_CLASS_TYPE(MouseHoverOff)
	};
}
