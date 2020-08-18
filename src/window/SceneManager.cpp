#include "SceneManager.h"
#include <graphics/Graphics.h>
#include <events/EventValidator.h>
#include <algorithm>

namespace mc
{
	static void RenderUIView(Ref<UIView>& view, UIView* parent_view);

	void SceneManager::DispatchEvent(EventPtr event)
	{
		m_EventQueue.push_back(event);
	}

	void SceneManager::AddView(Ref<UIView>& view)
	{
		m_Views.push_back(view);

		// Sorting the elements out
		std::sort(m_Views.begin(), m_Views.end(), [](Ref<UIView>& lhs, Ref<UIView>& rhs) -> bool {
			return lhs->GetZIndex() < rhs->GetZIndex();
		});
	}

	void SceneManager::RemoveView(Ref<UIView> view)
	{
		m_ViewsToDelete.push_back(view);
	}

	void SceneManager::RemoveAllViews()
	{
		for (auto& view : m_Views)
			RemoveView(view);
	}

	void SceneManager::RemoveNeededViews()
	{
		std::vector<Ref<UIView>> m_ViewsToDeleteClone;
		{
			std::lock_guard<std::mutex> guard(m_ViewsToDeleteMutex);
			m_ViewsToDeleteClone.swap(m_ViewsToDelete);
		}

		for (auto& view : m_ViewsToDeleteClone)
		{
			auto it = std::find(m_Views.begin(), m_Views.end(), view);
			if (it != m_Views.end())
				m_Views.erase(it);
		}
	}

	Ref<UIView> SceneManager::GetViewRef(UIView* raw_address)
	{
		for (auto view : m_Views)
			if (view.get() == raw_address)
				return view;

		return nullptr;
	}

	void SceneManager::RenderGraphics()
	{
		if (m_ViewsToDelete.size())
			RemoveNeededViews();

		for (auto& view : m_Views)
			RenderUIView(view, view->parent);
	}

	void SceneManager::ProcessEvents(uint32_t window_dpi)
	{
		std::vector<EventPtr> m_EventQueueClone;
		{
			std::lock_guard<std::mutex> guard(m_EventQueueMutex);
			m_EventQueueClone.swap(m_EventQueue);
		}

		std::vector<Ref<UIView>> m_ViewsListClone(m_Views);

		for (auto& event : m_EventQueueClone)
		{
			// Event handling needs to be done in reverse order compared to rendering views.
			// Top element must receive events first, but renderered last.
			for (auto it = m_ViewsListClone.rbegin(); it != m_ViewsListClone.rend(); ++it)
			{
				// If the event has not been handled yet, process it.
				// If the event got handled, break out of the loop and move on to the next event.
				if (!event->Handled)
					ProcessEvent(event, *it, m_ViewsListClone, window_dpi);
				else
					break;
			}
		}
	}

	void RenderUIView(Ref<UIView>& view, UIView* parent_view)
	{
		// Preserving initial constraints
		Layer initial_layer = view->layer;

		// Check if constraints are needed to be adjusted
		if (parent_view != nullptr)
		{
			// Adjusting current view's position in relation to the parent view container
			view->layer.frame.position.x += parent_view->layer.frame.position.x;
			view->layer.frame.position.y += parent_view->layer.frame.position.y;
		}

		// Setting the clipping layer for the view based on its constraints
		Graphics::PushLayer(
			view->layer.frame.position.x,
			view->layer.frame.position.y,
			view->layer.frame.size.width,
			view->layer.frame.size.height
		);

		// Draw view and all of its subviews ONLY IF it is visible.
		// Layer modifications with PushLayer and PopLayer are still required
		// even if the view is invisible.
		if (view->Visible)
		{
			// Rendering the view
			view->Draw();

			// Recursively rendering all subviews
			for (auto& subview : view->subviews)
				RenderUIView(subview, view.get());
		}

		// Restoring initial constraints
		view->layer = initial_layer;

		// Popping the clipping layer
		Graphics::PopLayer();
	}

	void SceneManager::ProcessEvent(EventPtr& event, Ref<UIView>& view, const std::vector<Ref<UIView>>& view_layer, uint32_t window_dpi)
	{
		// Validating event (checking if the event belongs to the view)
		bool event_valid = EventValidator::ValidateEvent(*event.get(), view, view_layer, window_dpi);

		// Retrieving all callback functions
		auto callbacks = view->GetEventHandlers(event->GetEventType());
		if (callbacks.size() > 0)
		{
			if (event_valid)
			{
				// For every callback, if it's valid, call the event handler
				for (auto callback : callbacks)
				{
					if (callback)
					{
						bool handled = callback(*event.get(), view.get());

						// If event was considered handled by at least one event handler,
						// then set the Handled flag to true.
						if (handled)
							event->Handled = true;
					}
				}
			}
		}

		// If the event was handled by the view, stop processing this event
		if (event->Handled) return;

		std::vector<Ref<UIView>> m_SubviewsListClone(view->subviews);

		// Process event for all subviews.
		// Again, events should be handled from the top-most element down to the beginning of the list.
		for (auto it = m_SubviewsListClone.rbegin(); it != m_SubviewsListClone.rend(); ++it)
		{
			if (!event->Handled)
				ProcessEvent(event, *it, m_SubviewsListClone, window_dpi);
			else
				break;
		}
	}
}
