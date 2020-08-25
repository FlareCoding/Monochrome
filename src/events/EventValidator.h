#pragma once
#include "../ui/UIView.h"
#include "Events.h"

namespace mc
{
	class EventValidator
	{
	public:
		static bool ValidateEvent(
			Event& event,
			Ref<UIView>& view,
			const std::vector<Ref<UIView>>& view_layer,
			uint32_t window_dpi
		);

	private:
		static bool ValidateMouseEvent(
			MouseEvent& event,
			Ref<UIView>& view,
			const std::vector<Ref<UIView>>& view_layer,
			uint32_t window_dpi
		);

		static bool ValidateKeyboardEvent(KeyboardEvent& event, Ref<UIView>& view);

	private:
		static bool IsMouseInsideViewFrame(Ref<UIView>& view, Point mouse_location, uint32_t window_dpi);

		static bool IsViewTopMost(
			Ref<UIView>& view,
			const std::vector<Ref<UIView>>& view_layer,
			Point mouse_location,
			uint32_t window_dpi
		);
	};
}
