#include "EventValidator.h"
#include <window/UIWindow.h>

namespace mc
{
	bool EventValidator::ValidateEvent(
		Event& event,
		Ref<UIView>& view,
		const std::vector<Ref<UIView>>& view_layer,
		uint32_t window_dpi)
	{
		//
		// Validate events based on their category
		//
		if (event.IsInCategory(EventCategory::EventCategoryMouse))
			return ValidateMouseEvent(static_cast<MouseEvent&>(event), view, view_layer, window_dpi);

		if (event.IsInCategory(EventCategory::EventCategoryKeyboard))
			return ValidateKeyboardEvent(static_cast<KeyboardEvent&>(event), view);

		return true;
	}

	bool EventValidator::ValidateMouseEvent(
		MouseEvent& event,
		Ref<UIView>& view,
		const std::vector<Ref<UIView>>& view_layer,
		uint32_t window_dpi)
	{
		bool MouseInsideFrame = IsMouseInsideViewFrame(view, event.location, window_dpi);

		// Determining if the view is on top of all other views in the current view layer
		bool ViewIsTopMost = IsViewTopMost(view, view_layer, event.location, window_dpi);

		// For mouse button press and release events,
		// if mouse location is not inside the view's frame
		// or the view is not on top of all other views, then it doesn't belong to that view.
		// In that case event would be automatically invalid.
		if (event.IsInCategory(EventCategory::EventCategoryMouseButton) && (!MouseInsideFrame || !ViewIsTopMost))
		{
			if (event.GetEventType() == EventType::MouseButtonPressed)
				if (view->srcwindow && view->srcwindow->GetFocusedView() == view.get())
					view->srcwindow->FocusView(nullptr);

			return false;
		}

		//
		// Checking for specific mouse events
		//

		// If mouse is moved, then we need to convert the distance to frame coordinate,
		// which aren't adjusted to any window's dpi.
		if (event.GetEventType() == EventType::MouseMoved)
		{
			// If mouse is not inside view's frame or view is not
			// at the top of everyone else, then event shouldn't be valid
			if (!MouseInsideFrame || !ViewIsTopMost)
				return false;

			static_cast<MouseMovedEvent&>(event).distance.x = static_cast<MouseMovedEvent&>(event).distance.x / (float)window_dpi * 96.0f;
			static_cast<MouseMovedEvent&>(event).distance.y = static_cast<MouseMovedEvent&>(event).distance.y / (float)window_dpi * 96.0f;
		}

		// For mouse HOVER ON event, event can only occur when mouse
		// first enters the view's frame and view's m_IsMouseOverFrame flag
		// is previously _off_.
		else if (event.GetEventType() == EventType::MouseHoverOn)
		{
			// If the event is not top-most, then it shouldn't receive hover _on_ event
			if (!ViewIsTopMost)
				return false;

			if (MouseInsideFrame && !view->m_IsMouseOverFrame)
				view->m_IsMouseOverFrame = true;
			else
				return false;

			// If the event is valid and view was hovered over for the first time,
			// then we need to trigger a Hover _Off_ event, go through all bottom views,
			// and manually call their event handlers.
			std::shared_ptr<MouseHoverOffEvent> manual_hover_off_event = nullptr;

			// Looping through each view in the layer
			for (auto& bottom_view : view_layer)
			{
				// Skip the main view as it is not at the bottom
				if (bottom_view.get() == view.get()) continue;

				// Check if view's IsMouseOverFrame flag is on, it would indicate
				// that mouse was over its frame during the last event check.
				if (bottom_view->m_IsMouseOverFrame)
				{
					// Turn off the view's m_IsMouseOverFrame flag since 
					// Hover _Off_ event will be called
					bottom_view->m_IsMouseOverFrame = false;

					// In order to not create the hover _off_ event in advance in case 
					// there is no need for it, we need to create it if it's needed and if
					// it is initially nullptr.
					if (manual_hover_off_event == nullptr)
					{
						manual_hover_off_event = std::make_shared<MouseHoverOffEvent>(
							event.location,
							static_cast<MouseHoverOnEvent&>(event).distance,
							static_cast<MouseHoverOnEvent&>(event).button
							);
					}

					// Get all hover _off_ event callback functions
					auto callbacks = bottom_view->GetEventHandlers<EventType::MouseHoverOff>();

					// Looping through each callback function and calling it.
					for (auto& callback : callbacks)
						if (callback)
							callback(*manual_hover_off_event.get(), bottom_view.get());
				}
			}
		}

		// For mouse HOVER OFF event, event can only occur when mouse
		// first exits the view's frame and view's m_IsMouseOverFrame flag
		// is already _on_.
		else if (event.GetEventType() == EventType::MouseHoverOff)
		{
			if (!MouseInsideFrame && view->m_IsMouseOverFrame)
				view->m_IsMouseOverFrame = false;
			else
				return false;
		}

		// If the event passed all checks and is valid, the element should become focused.
		// The view should also have focus changed event handlers.
		if (event.GetEventType() == EventType::MouseButtonPressed && view->GetEventHandlers<EventType::FocusChanged>().size())
		{
			if (view->srcwindow)
				view->srcwindow->FocusView(view);
		}

		return true;
	}

	bool EventValidator::ValidateKeyboardEvent(KeyboardEvent& event, Ref<UIView>& view)
	{
		if (view->srcwindow && view->srcwindow->IsViewFocused(view))
			return true;
		else
			return false;
	}

	bool EventValidator::IsMouseInsideViewFrame(Ref<UIView>& view, Point mouse_location, uint32_t window_dpi)
	{
		// Views' positions are relative to their parent views' positions,
		// so we need to construct the view's frame according to its 
		// absolute position.
		Frame absolute_frame = Frame(view->GetAbsolutePosition(), view->layer.frame.size);

		// Adjusting the frame coordinates to match the window's dpi settings to get accurate results
		absolute_frame.AdjustToDpi(window_dpi);

		// Determining if mouse was inside view's frame
		return absolute_frame.DoesContain(mouse_location);
	}

	bool EventValidator::IsViewTopMost(
		Ref<UIView>& view,
		const std::vector<Ref<UIView>>& view_layer,
		Point mouse_location,
		uint32_t window_dpi)
	{
		// Checking if views aren't the same view and if the view being checked
		// has a higher Z Index than the view being tested against, then element
		// is not at the top.
		for (auto& v : view_layer)
		{
			if (v.get() != view.get() && v->GetZIndex() > view->GetZIndex())
			{
				// Getting the absolute frame for the view that can potentially be on top
				Frame absolute_frame = Frame(v->GetAbsolutePosition(), v->layer.frame.size);

				// Adjusting the frame coordinates to match the window's dpi settings to get accurate results
				absolute_frame.AdjustToDpi(window_dpi);

				// If the view is on top and mouse is inside its frame as well,
				// then the current view that is being tested isn't the top-most
				if (absolute_frame.DoesContain(mouse_location))
					return false;
			}
		}

		return true;
	}
}
