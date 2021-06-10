#include "UIButton.h"
#include <graphics/Graphics.h>

namespace mc
{
	UIButton::UIButton()
	{
		layer.frame = Frame(40, 40, 180, 50);
		SetDefaultOptions();
	}

	UIButton::UIButton(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}

	void UIButton::SetDefaultOptions()
	{
		layer.color = Color(82, 87, 107, 1.0f);

		Label = MakeRef<UILabel>();
		Label->color = Color::white;
		Label->layer.frame = Frame(0, 0, layer.frame.size.width, layer.frame.size.height);
		Label->Text = "Button";
		subviews.push_back(Label);

		SetupEventHandlers();
	}

	void UIButton::SetupEventHandlers()
	{
		AddEventHandler<EventType::MouseButtonPressed>([this](Event& event, UIView* sender) -> bool {
			m_PreMousePressColor = layer.color;

			if (OnMousePressColor == Color::transparent)
			{
				layer.color.r -= (uint32_t)((float)layer.color.r * 0.3f);
				layer.color.g -= (uint32_t)((float)layer.color.g * 0.3f);
				layer.color.b -= (uint32_t)((float)layer.color.b * 0.3f);
			}
			else
				layer.color = OnMousePressColor;

			m_PostHoverOnColor = layer.color;
			m_IsMousePressed = true;
			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseButtonReleased>([this](Event& event, UIView* sender) -> bool {
			layer.color = m_PreMousePressColor;
			m_PostHoverOnColor = layer.color;

			m_IsMousePressed = false;
			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOn>([this](Event& event, UIView* sender) -> bool {
			m_PreHoverOnColor = layer.color;

			if (HoverOnColor == Color::transparent)
			{
				layer.color.r -= (uint32_t)((float)layer.color.r * 0.3f);
				layer.color.g -= (uint32_t)((float)layer.color.g * 0.3f);
				layer.color.b -= (uint32_t)((float)layer.color.b * 0.3f);
			}
			else
				layer.color = HoverOnColor;

			m_PostHoverOnColor = layer.color;
			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseHoverOff>([this](Event& event, UIView* sender) -> bool {
			if (layer.color == m_PostHoverOnColor)
				layer.color = m_PreHoverOnColor;
			
			m_PostHoverOnColor = Color::transparent;
			m_IsMousePressed = false;
			return EVENT_UNHANDLED;
		});
	}

	void UIButton::Update()
	{
		// Keeping the label frame same as button's
		Label->layer.frame = Frame(0, 0, layer.frame.size.width, layer.frame.size.height);

		// Controlling the opacity of the child label
		Label->layer.color.alpha = layer.color.alpha;
		Label->color.alpha = layer.color.alpha;
	}

	void UIButton::Draw()
	{
		Update();

		Graphics::DrawRectangle(
			layer.frame.position.x,
			layer.frame.position.y,
			layer.frame.size.width,
			layer.frame.size.height,
			layer.color,
			CornerRadius,
			Filled,
			Stroke
		);
	}
}
