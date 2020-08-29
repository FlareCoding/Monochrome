#pragma once
#include "UIScrollPanel.h"
#include <graphics/TextProperties.h>

namespace mc
{
	class UITextArea : public UIView
	{
	public:
		UITextArea();
        UITextArea(Frame frame);
        ~UITextArea() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		/// Text to be displayed in the text area.
		std::string Text = "";

		/// Font and style of the text in the label.
		TextProperties Properties;

		/// Color of the text.
		Color TextColor = Color::black;

        /// Padding between the top border and the text.
        float TopMargins = 2.0f;

        /// Padding between the bottom border and the text.
        float BottomMargins = 2.0f;

        /// Padding between the left border and the text.
        float LeftMargins = 2.0f;

        /// Padding between the right border and the text.
        float RightMargins = 2.0f;

        /// Will scroll the focused area to the bottom when the text is changed.
        bool AutoScroll = true;

    private:
        Ref<UIScrollPanel> m_ContentPanel;
        Ref<UILabel> m_TextSource;

        void SetDefaultOptions();
        void Update();

        void OnTextChanged();
        std::string m_PreviousText = "";
	};
}
