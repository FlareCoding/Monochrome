#pragma once
#include <ui/UIView.h>

namespace mc
{
    class UICircularProgressBar : public UIView
    {
    public:
        UICircularProgressBar();
        UICircularProgressBar(Frame frame);
        ~UICircularProgressBar() = default;

        // Inherited via IDrawable
        virtual void Draw() override;

        /// Line width of the main progress bar circle.
        float Stroke = 2.0f;

        /// Color of the part of the bar that indicates the progress.
        Color ProgressColor = Color::cyan;

        float MaxValue = 100.0f;
        float MinValue = 0.0f;
        float Value = 40.0f;

    public:
        using CircularProgressBarValueChangedCallbackFn = std::function<void(float value, UICircularProgressBar* sender)>;

        /// Adds a callback function that gets called every time the progress value changes.
        void AddValueChangedEventHandler(CircularProgressBarValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

        /// Adds a callback function that gets called every time the progress value changes.
        void operator +=(CircularProgressBarValueChangedCallbackFn cb) { m_ValueChangedCallbacks.push_back(cb); }

    private:
        std::vector<CircularProgressBarValueChangedCallbackFn> m_ValueChangedCallbacks;

    private:
        float m_PreviousValue = 0.0f;
        Point m_ArcEndPoint = { 0, 0 };
        bool m_IsLargeArc = false;

        float m_Angle = 0;
        float m_Radius = 0;
        float m_PercentCompleted = 0;
        
        void Update();
        void SetDefaultOptions();
    };
}
