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

		float	CornerRadius = 6;
		Color	ScrollbarColor = Color(40, 40, 40, 1.0f);

		/// Adds a view to the scrollable content area.
		void AddChild(const Ref<UIView>& view);

		/// View containing all the scrollable content.
		Ref<UIView> ContentView;

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

		void ScrollContent(float distance);

	private:
		float m_ScrollbarWidth = 10.0f;
		Ref<UIButton> m_VerticalScrollbar;

		float GetMaximumVerticalScrollAmount();
		float GetVerticalScrollAmount();
		void ProcessScrollbar();
		
		bool m_VerticalScrollbarMousePressed = false;
	};
}
