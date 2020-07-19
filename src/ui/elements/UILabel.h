#pragma once
#include <ui/UIView.h>
#include <graphics/TextProperties.h>

namespace mc
{
	class UILabel : public UIView
	{
	public:
		UILabel() = default;
		~UILabel() = default;

		UILabel(Frame frame);

		// Inherited via IDrawable
		virtual void Draw() override;

		std::string Text = "Label";
		TextProperties Properties;

		Color color = Color::black;
	};
}
