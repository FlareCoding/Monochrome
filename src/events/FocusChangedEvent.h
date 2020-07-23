#pragma once
#include "Event.h"

namespace mc
{
	/// Event that is fired whenever a UIView gains or loses focus.
	class FocusChangedEvent : public Event
	{
	public:
		FocusChangedEvent(bool gained_focus) : GainedFocus(gained_focus) {}

		/// Holds the mouse position in the window. 
		const bool GainedFocus;

		REGISTER_EVT_CATEGORY(EventCategoryInput)
		REGISTER_EVT_CLASS_TYPE(FocusChanged)
	};
}
