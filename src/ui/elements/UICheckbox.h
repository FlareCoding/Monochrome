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
		
		/// Color of the checkmark that appears when the checkbox is checked.
		Color	CheckmarkColor = Color::black;

		/// Color of the checkbox when it's checked.
		Color	CheckedBoxColor = Color::light_blue;

		/// Label with a text next to the checkbox.
		Ref<UILabel> Label;

		bool Checked = false;

	public:
		using CheckboxValueChangedCallbackFn = std::function<void(bool checked, UICheckbox* sender)>;

		/// Adds a callback function that gets called every time the value of the checkbox changes.
		void AddValueChangedEventHandler(CheckboxValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

		/// Adds a callback function that gets called every time the value of the checkbox changes.
		void operator +=(CheckboxValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

	private:
		void SetDefaultOptions();
		void SetupEventHandlers();
		void Update();

		bool m_PreviousValue = Checked;

		std::vector<CheckboxValueChangedCallbackFn> m_ValueChangedCallbacks;
	};
}
