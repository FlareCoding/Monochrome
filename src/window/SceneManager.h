#pragma once
#include <ui/UIView.h>
#include <mutex>

namespace mc
{
	class SceneManager
	{
	public:
		void RenderGraphics();
		void ProcessEvents(uint32_t window_dpi);

	public:
		void DispatchEvent(EventPtr event);
		void AddView(Ref<UIView>& view);
		void RemoveView(Ref<UIView> view);
		Ref<UIView> GetViewRef(UIView* raw_address);

	private:
		void ProcessEvent(EventPtr& event, Ref<UIView>& view, const std::vector<Ref<UIView>>& view_layer, uint32_t window_dpi);

	private:
		std::vector<Ref<UIView>> m_Views;
		std::vector<EventPtr> m_EventQueue;
		std::mutex m_EventQueueMutex;
	};
}
