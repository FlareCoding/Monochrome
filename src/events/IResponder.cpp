#include "IResponder.h"

namespace mc
{
	void IResponder::AddEventHandler(EventType type, EventCallbackFn handler)
	{
		auto query = m_Events.find(type);
		if (query == m_Events.end())
		{
			std::vector<EventCallbackFn> handlers;
			handlers.push_back(handler);
			m_Events.insert(std::pair<EventType, std::vector<EventCallbackFn>>(type, handlers));
		}
		else
			query->second.push_back(handler);
	}

	void IResponder::operator+=(std::pair<EventType, EventCallbackFn> handler)
	{
		AddEventHandler(handler.first, handler.second);
	}

	std::vector<IResponder::EventCallbackFn> IResponder::GetEventHandlers(EventType type)
	{
		static std::vector<EventCallbackFn> _empty_;
		auto query = m_Events.find(type);

		if (query != m_Events.end())
			return query->second;
		else
			return _empty_;
	}

	bool IResponder::HasEventHandlers()
	{
		return m_Events.size();
	}
}