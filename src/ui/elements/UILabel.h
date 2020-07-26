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

		/// Text to be displayed in the label.
		std::string Text = "Label";

		/// Optional widestring text to be displayed in the label instead of the usual string.
		std::wstring WidestringText = L"";

		/// Specifies whether to use widestring text instead of the usual string.
		bool UseWidestringText = false;

		/// Font and style of the text in the label.
		TextProperties Properties;

		/// Color of the text.
		Color color = Color::black;
	};
}
