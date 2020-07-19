#pragma once
#include "Event.h"
#include <vector>
#include <map>
#include <functional>

namespace mc
{
	class UIView;

	class IResponder
	{
	public:
		using EventCallbackFn = std::function<bool(Event&, UIView* sender)>;

		template <EventType ET>
		void AddEventHandler(EventCallbackFn handler);

		template <EventType ET>
		std::vector<EventCallbackFn> GetEventHandlers();

		void AddEventHandler(EventType type, EventCallbackFn handler);
		void operator +=(std::pair<EventType, EventCallbackFn> handler);

		std::vector<EventCallbackFn> GetEventHandlers(EventType type);

	private:
		std::map<EventType, std::vector<EventCallbackFn>> m_Events;
	};

	template<EventType ET>
	inline void IResponder::AddEventHandler(EventCallbackFn handler)
	{
		AddEventHandler(ET, handler);
	}

	template<EventType ET>
	inline std::vector<IResponder::EventCallbackFn> IResponder::GetEventHandlers()
	{
		return GetEventHandlers(ET);
	}
}
