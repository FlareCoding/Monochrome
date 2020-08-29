#pragma once
#include <ui/UIView.h>
#include <ui/elements/UIButton.h>

namespace mc
{
	class UIScrollPanel : public UIView
	{
	public:
		UIScrollPanel();
		UIScrollPanel(Frame frame);
		~UIScrollPanel() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		Color	ScrollbarColor = Color(40, 40, 40, 1.0f);

		/// Adds a view to the scrollable content area.
		void AddChild(const Ref<UIView>& view);

		/// Removes a view from the scrollable content area.
		void RemoveChild(Ref<UIView> view);

		/// Removes all views from the scrollable content area.
		void Clear();

		/// View containing all the scrollable content.
		Ref<UIView> ContentView;

		/// Scrolls the content to the top-most area.
		void ScrollToTop();

		/// Scrolls the content to the bottom-most area.
		void ScrollToBottom();

		/// Scrolls the content by the specified amount in pixels.
		void ScrollContent(float distance);

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

	private:
		float m_ScrollbarWidth = 10.0f;
		Ref<UIButton> m_VerticalScrollbar;

		float GetMaximumVerticalScrollAmount();
		float GetVerticalScrollAmount();
		void ProcessScrollbar();
		
		bool m_IsFocused = false;
		bool m_VerticalScrollbarMousePressed = false;
	};
}
