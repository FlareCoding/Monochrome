#include "UISlider.h"
#include <graphics/Graphics.h>
#include <events/Events.h>
#include <window/UIWindow.h>

namespace mc
{
	static float UISLIDER_RECTANGULAR_KNOB_WIDTH = 7.0f;

	Frame UISlider::GetSliderKnobAbsoluteBoundingBox()
	{	
		Point box_location;
		Size  box_size;

		float slider_area_start_offset = (m_SliderAreaStart - layer.frame.position.x);

		if (SliderKnobShape == Shape::Circle)
		{
			float box_radius = SliderBarHeight * 2.0f;

			box_location.x = layer.frame.position.x + slider_area_start_offset + m_SliderKnobPositionOffset - box_radius;
			box_location.y = GetAbsolutePosition().y + (layer.frame.size.height / 2.0f) + (SliderBarHeight / 2.0f) - box_radius;

			box_size.width = box_radius * 2.0f;
			box_size.height = box_radius * 2.0f;
		}
		else
		{
			box_location.x = layer.frame.position.x + slider_area_start_offset + m_SliderKnobPositionOffset;
			box_location.y = GetAbsolutePosition().y;

			box_size.width = UISLIDER_RECTANGULAR_KNOB_WIDTH;
			box_size.height = layer.frame.size.height;
		}

		auto box = Frame(box_location, box_size);
		
		auto window_dpi = UIWindow::GetCurrentActiveWindow()->GetDpi();
		box.AdjustToDpi(window_dpi);

		return box;
	}

	float UISlider::GetKnobOffsetFromAbsoluteMousePos(Point mouse_pos, bool& value_changed)
	{
		float slider_area_start_offset = (m_SliderAreaStart - layer.frame.position.x);

		float mouse_offset = mouse_pos.x - ((layer.frame.position.x + slider_area_start_offset) * UIWindow::GetCurrentActiveWindow()->GetDpi() / 96.0f);
		float percentage = mouse_offset / m_SliderAreaSize;
		float precise_value = percentage * MaxValue;

		float old_value = Value;
		Value = roundf(precise_value / Intervals) * Intervals;

		// Value correction to keep it in bounds
		if (Value > MaxValue)
			Value = MaxValue;
		else if (Value < MinValue)
			Value = MinValue;

		value_changed = (Value != old_value);

		return m_SliderAreaSize * (Value / MaxValue);
	}

	UISlider::UISlider()
	{
		layer.frame = Frame(40, 40, 180, 50);
		SetDefaultOptions();
	}

	UISlider::UISlider(Frame frame) : UIView(frame)
	{
		SetDefaultOptions();
	}
	
	void UISlider::SetDefaultOptions()
	{
		CornerRadius = 0;

		layer.color = Color(82, 87, 107, 1.0f);

		m_SliderAreaStart = CornerRadius * 2.0f;
		m_SliderAreaEnd = layer.frame.size.width - CornerRadius * 2.0f;
		m_SliderAreaSize = m_SliderAreaEnd - m_SliderAreaStart;

		// artificially setting the knob position to the middle
		Update();
		m_SliderKnobPositionOffset = m_SliderAreaStart + m_TickmarkIntervals * (int)(m_TickmarkCount / 2);

		SetupEventHandlers();
	}

	void UISlider::SetupEventHandlers()
	{
		AddEventHandler<EventType::MouseButtonPressed>([this](Event& event, UIView* sender) -> bool {

			if (((MouseButtonEvent&)event).button == MouseButton::Left)
			{
				auto box_frame = GetSliderKnobAbsoluteBoundingBox();

				if (box_frame.DoesContain(((MouseButtonEvent&)event).location))
					m_bMousePressedOnKnob = true;
			}

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseButtonReleased>([this](Event& event, UIView* sender) -> bool {

			if (((MouseButtonEvent&)event).button == MouseButton::Left)
				m_bMousePressedOnKnob = false;

			return EVENT_UNHANDLED;
		});

		AddEventHandler<EventType::MouseMoved>([this](Event& event, UIView* sender) -> bool {
			MouseMovedEvent& mmevent = (MouseMovedEvent&)event;

			if (mmevent.button == MouseButton::Left && m_bMousePressedOnKnob)
			{
				bool value_changed = false;
				m_SliderKnobPositionOffset = GetKnobOffsetFromAbsoluteMousePos(mmevent.location, value_changed);

				// If the slider's value was changed, call the callback functions
				if (value_changed)
				{
					for (auto& cb : m_ValueChangedCallbacks)
						cb(Value, this);

					m_PreviousValue = Value;
				}
			}

			return EVENT_UNHANDLED;
		});
	}

	void UISlider::Update()
	{
		// Value correction to keep it in bounds
		if (Value > MaxValue)
			Value = MaxValue;
		else if (Value < MinValue)
			Value = MinValue;

		// Value rounding in case it doesn't match intervals
		float percentage = Value / MaxValue;
		Value = roundf(percentage * MaxValue / Intervals) * Intervals;

		// If value was changed programmatically by the user, update the knob position
		bool value_changed = (Value != m_PreviousValue);
		if (value_changed)
		{
			for (auto& cb : m_ValueChangedCallbacks)
				cb(Value, this);

			m_SliderKnobPositionOffset = m_SliderAreaSize * (Value / MaxValue);
			m_PreviousValue = Value;
		}

		// Calculating UI related content
		float whitespace = SliderBarHeight * 4.0f; // Knob Radius = SliderBarHeight * 2.0f

		m_SliderAreaStart = layer.frame.position.x + whitespace;
		m_SliderAreaEnd = layer.frame.position.x + layer.frame.size.width - whitespace;
		m_SliderAreaSize = m_SliderAreaEnd - m_SliderAreaStart;

		float value_range = MaxValue - MinValue;
		m_TickmarkCount = (int)(value_range / Intervals);
		m_TickmarkIntervals = m_SliderAreaSize / (float)m_TickmarkCount;
	}

	void UISlider::Draw()
	{
		Update();

		// Slider bar
		Graphics::DrawRectangle(
			m_SliderAreaStart,
			layer.frame.position.y + layer.frame.size.height / 2.0f,
			m_SliderAreaSize,
			SliderBarHeight,
			layer.color,
			CornerRadius,
			true
		);

		// Slider knob
		if (SliderKnobShape == Shape::Circle)
		{
			Graphics::DrawCircle(
				m_SliderAreaStart + m_SliderKnobPositionOffset,
				layer.frame.position.y + (layer.frame.size.height / 2.0f) + (SliderBarHeight / 2.0f),
				SliderBarHeight * 2.0f,
				SliderKnobColor
			);
		}
		else
		{
			Graphics::DrawRectangle(
				m_SliderAreaStart + m_SliderKnobPositionOffset,
				layer.frame.position.y,
				UISLIDER_RECTANGULAR_KNOB_WIDTH,
				layer.frame.size.height,
				SliderKnobColor
			);
		}

		// Tick marks
		if (VisibleTickmarks)
		{
			for (int i = 0; i < m_TickmarkCount + 1; i++)
			{
				float tickmark_x_pos = m_SliderAreaStart + m_TickmarkIntervals * i;
				if (i == m_TickmarkCount)
					tickmark_x_pos -= 1;

				Graphics::DrawRectangle(
					tickmark_x_pos,
					layer.frame.position.y + layer.frame.size.height * 0.3f,
					1.0f,
					layer.frame.size.height - layer.frame.size.height * 0.3f,
					TickmarksColor
				);
			}
		}
	}
}
