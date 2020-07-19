#pragma once
#include "UILabel.h"

namespace mc
{
	class UIButton : public UIView
	{
	public:
		UIButton();
		UIButton(Frame frame);
		~UIButton() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		float	CornerRadius = 6;
		bool	Filled = true;
		float	Stroke = 2;

		Color HoverOnColor = Color::transparent;
		Color OnMousePressColor = Color::transparent;

		Ref<UILabel> Label;

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

	private:
		Color m_PreHoverOnColor = { 0, 0, 0, 0 };
		Color m_PreMousePressColor = { 0, 0, 0, 0 };
	};
}
