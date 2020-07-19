#include "UICheckbox.h"
#include <graphics/Graphics.h>
#include <events/Events.h>
#include <window/UIWindow.h>

namespace mc
{
	static float UICHECKBOX_BOX_LEADING_MARGIN = 6;

	UICheckbox::UICheckbox()
	{
		layer.frame = Frame(40, 40, 180, 50);
		SetDefaultOptions();
	}

	UICheckbox::UICheckbox(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UICheckbox::SetDefaultOptions()
	{
		Label = MakeRef<UILabel>();
		Label->color = Color::white;
		Label->layer.frame = Frame(BoxSize + LabelMargins, 0, layer.frame.size.width - BoxSize, layer.frame.size.height);
		Label->Text = "Checkbox";
		Label->Properties.Allignment = TextAlignment::LEADING;
		subviews.push_back(Label);

		SetupEventHandlers();
	}

	void UICheckbox::SetupEventHandlers()
	{
		AddEventHandler<EventType::MouseButtonReleased>([this](Event& event, UIView* sender) -> bool {
			auto& e = (MouseButtonEvent&)event;

			auto window_dpi = UIWindow::GetCurrentActiveWindow()->GetDpi();

			Point box_location;
			box_location.x = (sender->GetAbsolutePosition().x + UICHECKBOX_BOX_LEADING_MARGIN) * (float)window_dpi / 96.0f;
			box_location.y = (sender->GetAbsolutePosition().y + (layer.frame.size.height / 2) - (BoxSize / 2)) * (float)window_dpi / 96.0f;

			float real_box_size = BoxSize * (float)window_dpi / 96.0f;
			Frame box_frame = Frame(Position { box_location.x, box_location.y }, Size { real_box_size, real_box_size });

			if (box_frame.DoesContain(e.location))
				Toggle();

			return EVENT_UNHANDLED;
		});
	}

	void UICheckbox::Toggle()
	{
		Checked = !Checked;
	}

	void UICheckbox::Update()
	{
		Label->layer.frame = Frame(BoxSize + LabelMargins, 0, layer.frame.size.width - BoxSize, layer.frame.size.height);

		// Controlling the opacity of the child label
		Label->layer.color.alpha = layer.color.alpha;
		Label->color.alpha = layer.color.alpha;

		// If the value was programmatically changed, call the callback handlers
		if (m_PreviousValue != Checked)
		{
			for (auto& cb : m_ValueChangedCallbacks)
				cb(Checked, this);

			m_PreviousValue = Checked;
		}
	}

	void UICheckbox::Draw()
	{
		Update();

		// Drawing the box
		Graphics::DrawRectangle(
			layer.frame.position.x + UICHECKBOX_BOX_LEADING_MARGIN,
			layer.frame.position.y + (layer.frame.size.height / 2) - (BoxSize / 2),
			BoxSize,
			BoxSize,
			Checked ? CheckedBoxColor : layer.color,
			CornerRadius,
			true,
			1
		);

		// Drawing the checkmark if neccessary
		if (Checked)
		{
			Graphics::DrawTextWideString(
				layer.frame.position.x + UICHECKBOX_BOX_LEADING_MARGIN,
				layer.frame.position.y + (layer.frame.size.height / 2) - (BoxSize / 2),
				BoxSize,
				BoxSize,
				L"✔",
				TextProperties(),
				CheckmarkColor
			);
		}
	}
}
