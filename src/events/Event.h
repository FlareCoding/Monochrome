#pragma once
#include "../Core/Core.h"

#include <string>
#include <sstream>

#define BIT(x) (1 << x)

namespace mc
{
#define EVENT_HANDLED	true;
#define EVENT_UNHANDLED false;

	enum class EventType
	{
		None = 0,
		MouseButtonPressed, MouseButtonReleased, MouseButtonClicked = MouseButtonReleased, MouseMoved, MouseHoverOn, MouseHoverOff, MouseScrolled,
		KeyDown, KeyPressed = KeyDown, KeyReleased
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryWindow			= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

#define REGISTER_EVT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define REGISTER_EVT_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

#pragma warning( push )
#pragma warning( disable : 26812 )
		inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
#pragma warning( pop ) 
	};

	using EventPtr = Ref<Event>;
}
