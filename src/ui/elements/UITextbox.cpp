#include "UITextbox.h"
#include <graphics/Graphics.h>
#include <window/UIWindow.h>
#include <string.h>

#if defined(_WIN32)
	#include <Windows.h>
#endif

namespace mc
{
	static std::string GetClipboardText()
	{
#if defined(_WIN32)
		// Try opening the clipboard
		if (!OpenClipboard(nullptr))
			return "";

		// Get handle of clipboard object for ANSI text
		HANDLE hData = GetClipboardData(1);
		if (hData == nullptr)
			return "";

		// Lock the handle to get the actual text pointer
		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (pszText == nullptr)
			return "";

		// Save text in a string class instance
		std::string text(pszText);

		// Release the lock
		GlobalUnlock(hData);

		// Release the clipboard
		CloseClipboard();

		return text;
#else
		return "";
#endif
	}

	UITextbox::UITextbox()
	{
		layer.frame = Frame(40, 40, 180, 26);
		SetDefaultOptions();
	}

	UITextbox::UITextbox(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UITextbox::SetDefaultOptions()
	{
		cursor = CursorType::IBeam;

		Text = "";
		Placeholder = "Enter text ...";
		textProperties.Allignment = TextAlignment::LEADING;
		textProperties.Wrapping = WordWrapping::NO_WRAP;

		m_Label = MakeRef<UILabel>();
		m_Label->layer.frame = Frame(5, 2, layer.frame.size.width - 10, layer.frame.size.height - 4);
		subviews.push_back(m_Label);

		Update();
		SetupEventHandlers();
	}

	void UITextbox::SetupEventHandlers()
	{
		AddEventHandler<EventType::KeyPressed>([this](Event& event, UIView* sender) -> bool {
			if (ReadOnly)
				return EVENT_UNHANDLED;

			KeyPressedEvent& evt = reinterpret_cast<KeyPressedEvent&>(event);
			auto input = std::string(1, McKeycodeToChar(evt.keycode, evt.capital, evt.capslock_on));

			if (!input.empty() && input != "\n")
				ProcessKeyEvent(input, evt.keycode);

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::KeyReleased>([this](Event& event, UIView* sender) -> bool {
			if (ReadOnly)
				return EVENT_UNHANDLED;

			KeyReleasedEvent& evt = reinterpret_cast<KeyReleasedEvent&>(event);
			if (evt.keycode == KeyCode::KEY_LCONTROL)
				m_CtrlPressed = false;

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::FocusChanged>([this](Event& event, UIView* sender) -> bool {
			if (ReadOnly)
				return EVENT_UNHANDLED;

			m_IsFocused = ((FocusChangedEvent&)event).GainedFocus;
			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseButtonPressed>([this](Event& event, UIView* sender) -> bool {
			if (ReadOnly)
				return EVENT_UNHANDLED;

			if (m_FirstTimeClick)
			{
				if (Text.size()) m_CursorIndex = Text.size();
				m_FirstTimeClick = false;
			}

			return EVENT_UNHANDLED;
		});
	}

	void UITextbox::ProcessKeyEvent(const std::string& input, KeyCode keycode)
	{
		switch (keycode)
		{
		case KeyCode::KEY_LCONTROL:
		{
			m_CtrlPressed = true;
			break;
		}
		case KeyCode::KEY_LEFT:
		{
			if (m_CursorIndex > 0)
			{
				if (m_CursorIndex == m_VisibleStartIndex)
				{
					m_VisibleStartIndex--;
					m_VisibleEndIndex--;
				}

				m_CursorIndex--;
			}
			break;
		}
		case KeyCode::KEY_RIGHT:
		{
			if (m_CursorIndex < Text.size())
			{
				if (m_CursorIndex == m_VisibleEndIndex)
				{
					m_VisibleStartIndex++;
					m_VisibleEndIndex++;
				}

				m_CursorIndex++;
			}
			break;
		}
		case KeyCode::KEY_BACKSPACE:
		{
			if (m_CursorIndex > 0)
			{
				Text.erase(m_CursorIndex - 1, 1);
				m_CursorIndex--;

				if (m_VisibleStartIndex > 0)
				{
					m_VisibleStartIndex--;
					m_VisibleEndIndex--;
				}
			}
			break;
		}
		default:
			if (keycode == KeyCode::KEY_V && m_CtrlPressed)
			{
				std::string ClipboardText = GetClipboardText();
				if (!ClipboardText.empty())
				{
					Text.insert(m_CursorIndex, ClipboardText);
					m_CursorIndex += ClipboardText.size();
					return;
				}
			}

			if (strcmp(input.c_str(), "\0") != 0)
			{
				Text.insert(m_CursorIndex, input);
				m_CursorIndex++;

				if (m_CursorIndex >= Text.size() || m_CursorIndex > m_VisibleEndIndex)
					RecalculateVisibleText();
			}
		}
	}

	void UITextbox::Draw()
	{
		Update();

		// Highlighted border when the textbox is in a focused state
		if (m_IsFocused)
		{
			Graphics::DrawRectangle(
				layer.frame.position.x,
				layer.frame.position.y,
				layer.frame.size.width,
				layer.frame.size.height,
				FocusedHighlightColor,
				CornerRadius,
				true,
				Stroke
			);
		}

		// Main textbox area
		// The offset is set to make the main area slightly smaller
		// to fit inside the highlighted area while the highlighted
		// border can still fit inside the layer's area.
		Graphics::DrawRectangle(
			layer.frame.position.x + 1,
			layer.frame.position.y + 1,
			layer.frame.size.width - 2,
			layer.frame.size.height - 2,
			layer.color,
			CornerRadius,
			true,
			Stroke
		);

		std::string PreCursorText = Text.substr(m_VisibleStartIndex, m_CursorIndex - m_VisibleStartIndex);
		auto metrics = Graphics::CalculateTextMetrics(PreCursorText, textProperties, layer.frame.size.width - 2, layer.frame.size.height - 2);

		// If the textbox is focused and active, draw the cursor
		if (m_IsFocused)
		{
			float TextToLayerGap = (layer.frame.size.height - metrics.Height) / 2.0f;

			Graphics::DrawLine(
				layer.frame.position.x + metrics.WidthIncludingTrailingWhitespace + 1,
				layer.frame.position.y + TextToLayerGap,
				layer.frame.position.x + metrics.WidthIncludingTrailingWhitespace + 1,
				layer.frame.position.y + layer.frame.size.height - TextToLayerGap,
				TextColor,
				1
			);
		}
	}

	void UITextbox::Update()
	{
		// Someone edited the text either through copy-pasting or
		// through the source code.
		if (std::labs((long)Text.size() - (long)m_OldText.size()) > 1)
		{
			m_CursorIndex = Text.size();
			RecalculateVisibleText();
		}

		// Sanitizing the cursor position
		if (m_CursorIndex > Text.size() && m_CursorIndex > 0)
			m_CursorIndex = Text.size() - 1;

		if (m_CursorIndex < 0)
			m_CursorIndex = 0;

		// Sanitizing visible text indices
		SanitizeVisibleText();

		// Keeping the label frame same as button's
		m_Label->layer.frame = Frame(5, 2, layer.frame.size.width - 10, layer.frame.size.height - 4);

		// Controlling the opacity of the child label
		m_Label->color = TextColor;
		m_Label->layer.color.alpha = layer.color.alpha;
		m_Label->color.alpha = layer.color.alpha;

		// Keeping label's text properties updated
		m_Label->Properties = textProperties;

		// Controlling the label's text and opacity in case
		// the placeholder text is displayed.
		if (Text.empty())
		{
			m_Label->Text = Placeholder;
			m_Label->color.alpha = 0.6f;
		}
		else
			m_Label->Text = m_VisibleText;

		m_OldText = Text;
	}

	void UITextbox::RecalculateVisibleText()
	{
		// Extra edge-case sanitization
		if (m_VisibleStartIndex > Text.size() || m_VisibleEndIndex < 0)
			return;

		bool success = false;
		while (!success)
		{
			auto VisibleText = Text.substr(m_VisibleStartIndex, m_CursorIndex - m_VisibleStartIndex);
			auto metrics = Graphics::CalculateTextMetrics(VisibleText, textProperties, layer.frame.size.width - 2, layer.frame.size.height - 2);

			if (!IsTextAboveLengthLimit(metrics.WidthIncludingTrailingWhitespace))
				success = true;
			else
			{
				m_VisibleStartIndex++;
				m_VisibleEndIndex++;
			}
		}

		m_VisibleEndIndex = m_CursorIndex;
	}

	void UITextbox::SanitizeVisibleText()
	{
		auto metrics = Graphics::CalculateTextMetrics(Text, textProperties, layer.frame.size.width - 2, layer.frame.size.height - 2);

		if (!IsTextAboveLengthLimit(metrics.WidthIncludingTrailingWhitespace))
		{
			m_VisibleStartIndex = 0;
			m_VisibleEndIndex = Text.size();
		}
		else
		{
			if (m_VisibleStartIndex < 0)
				m_VisibleStartIndex = 0;

			if (m_VisibleEndIndex > Text.size())
				m_VisibleEndIndex = Text.size();
		}

		m_VisibleText = Text.substr(m_VisibleStartIndex, m_VisibleEndIndex - m_VisibleStartIndex);
	}

	bool UITextbox::IsTextAboveLengthLimit(float text_width)
	{
		static float text_safe_area_gap = 10.0f;
		return text_width >= (layer.frame.size.width - text_safe_area_gap);
	}
}
