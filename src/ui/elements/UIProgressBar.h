#pragma once
#include <ui/UIView.h>

namespace mc
{
    class UIProgressBar : public UIView
	{
	public:
		UIProgressBar();
        UIProgressBar(Frame frame);
        ~UIProgressBar() = default;

		// Inherited via IDrawable
		virtual void Draw() override;

        /// Indicates whether to fill the actual progress bar frame rectangle when drawing.
        bool Filled = true;

        /// If the Filled property is set to false, this is the stroke value that will be used.
        float Stroke = 2.0f;

        /// Color of the part of the bar that indicates the progress.
        Color ProgressColor = Color::cyan;

		float MaxValue = 100.0f;
		float MinValue = 0.0f;
		float Value = 40.0f;
    
    public:
		using ProgressBarValueChangedCallbackFn = std::function<void(float value, UIProgressBar* sender)>;

		/// Adds a callback function that gets called every time the progress value changes.
		void AddValueChangedEventHandler(ProgressBarValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

		/// Adds a callback function that gets called every time the progress value changes.
		void operator +=(ProgressBarValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

    private:
		std::vector<ProgressBarValueChangedCallbackFn> m_ValueChangedCallbacks;

    private:
        float m_PreviousValue = 0.0f;
        void Update();
        void SetDefaultOptions();
    };
}
