#pragma once
#include "Event.h"
#include <vector>
#include <map>
#include <functional>

namespace mc
{
	class UIView;

	/// Base class for any element that can receive system events.
	class IResponder
	{
	public:
		using EventCallbackFn = std::function<bool(Event&, UIView* sender)>;

		/// Registers an event callback handler.
		///
		/// @param handler callback function to be registered
		///
		template <EventType ET>
		void AddEventHandler(EventCallbackFn handler);

		/// Registers an event callback handler.
		///
		/// @param handler callback function to be registered
		///
		void AddEventHandler(EventType type, EventCallbackFn handler);

		/// Registers an event callback handler.
		///
		/// @param handler callback function to be registered
		///
		void operator +=(std::pair<EventType, EventCallbackFn> handler);

		/// Returns a list of all registered callbacks for the responder.
		template <EventType ET>
		std::vector<EventCallbackFn> GetEventHandlers();

		/// Returns a list of all registered callbacks for the responder.
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
