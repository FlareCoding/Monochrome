#pragma once
#include "UILabel.h"

namespace mc
{
	class UICheckbox : public UIView
	{
	public:
		UICheckbox();
		UICheckbox(Frame frame);
		~UICheckbox() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

		// Toggles the checkbox's value
		void Toggle();

		float	BoxSize = 16;
		float	LabelMargins = 16;
		float	CornerRadius = 2;
		
		Color	CheckmarkColor = Color::black;
		Color	CheckedBoxColor = Color::light_blue;

		Ref<UILabel> Label;

		bool Checked = false;

	public:
		using CheckboxValueChangedCallbackFn = std::function<void(bool checked, UICheckbox* sender)>;

		// Adds a callback function that gets called every time the value of the slider changes
		void AddValueChangedEventHandler(CheckboxValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }
		void operator +=(CheckboxValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

		bool m_PreviousValue = Checked;

		std::vector<CheckboxValueChangedCallbackFn> m_ValueChangedCallbacks;
	};
}
