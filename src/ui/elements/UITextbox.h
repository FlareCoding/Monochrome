#pragma once
#include "UILabel.h"
#include <events/KeyboardEvents.h>
#include <events/FocusChangedEvent.h>

namespace mc
{
    class UITextbox : public UIView
    {
    public:
		UITextbox();
		UITextbox(Frame frame);
		~UITextbox() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		float	Stroke = 2;

		/// Color of the border around the textbox when it is in a focused state.S
		Color	FocusedHighlightColor = Color::cyan;

		/// Text inside the textbox.
		std::string Text;

		/// Placeholder text that will be displayed if the textbox is empty.
		std::string Placeholder;

		/// Font and style of the text or placeholder in the textbox.
		TextProperties textProperties;

		/// Color of the text.
		Color TextColor = Color::black;

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void ProcessKeyEvent(const std::string& input, KeyCode keycode);

		Ref<UILabel> m_Label;

		bool m_IsFocused = false;

		// Used to shift cursor to the end of the text
		// when textbox is clicked for the first time.
		bool m_FirstTimeClick = true;

		bool m_CtrlPressed = false;

	private:
		void Update();

	private:
		size_t m_CursorIndex = 0;
		size_t m_VisibleStartIndex = 0;
		size_t m_VisibleEndIndex = 0;

		std::string m_VisibleText = "";
		std::string m_OldText = "";

		void RecalculateVisibleText();
		void SanitizeVisibleText();
		bool IsTextAboveLengthLimit(float text_width);
    };
}
