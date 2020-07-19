#pragma once
#include "UIButton.h"
#include <graphics/Shapes.h>

namespace mc
{
	class UISlider : public UIView
	{
	public:
		UISlider();
		UISlider(Frame frame);
		~UISlider() = default;
		
		// Inherited via IDrawable
		virtual void Draw() override;

		Shape SliderKnobShape = Shape::Rectangle;

		float CornerRadius = 0;
		float SliderBarHeight = 4.0f;

		Color SliderKnobColor = Color::gray;
		Color TickmarksColor = Color::gray;

		float MaxValue = 100.0f;
		float MinValue = 0.0f;
		float Value = 50.0f;
		float Intervals = 10.0f;
		bool  VisibleTickmarks = true;

	public:
		using SliderValueChangedCallbackFn = std::function<void(float value, UISlider* sender)>;

		// Adds a callback function that gets called every time the value of the slider changes
		void AddValueChangedEventHandler(SliderValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }
		void operator +=(SliderValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

	private:
		Frame GetSliderKnobAbsoluteBoundingBox();
		float GetKnobOffsetFromAbsoluteMousePos(Point mouse_pos, bool& value_changed);

	private:
		float m_SliderAreaStart = 0;
		float m_SliderAreaEnd = 0;
		float m_SliderAreaSize = 0;
	
		float m_SliderKnobPositionOffset = 0;
		int   m_TickmarkCount = 0;
		float m_TickmarkIntervals = 0;

		float m_PreviousValue = Value;

		bool  m_bMousePressedOnKnob = false;

	private:
		std::vector<SliderValueChangedCallbackFn> m_ValueChangedCallbacks;
	};
}
