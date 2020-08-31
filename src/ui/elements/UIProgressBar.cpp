#include "UIProgressBar.h"
#include <graphics/Graphics.h>

namespace mc
{
    UIProgressBar::UIProgressBar()
	{
        layer.frame = Frame(40, 100, 160, 6);
        SetDefaultOptions();
	}

	UIProgressBar::UIProgressBar(Frame frame) : UIView(frame)
	{
        SetDefaultOptions();
	}

    void UIProgressBar::SetDefaultOptions()
    {
        CornerRadius = 2.0f;
        layer.color = Color::white;
    }

	void UIProgressBar::Draw()
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

        float percent_completed = Value / MaxValue;
        if (percent_completed < 0)
            percent_completed = 0;
        else if (percent_completed > 1)
            percent_completed = 1;

        Graphics::DrawRectangle(
            layer.frame.position.x,
            layer.frame.position.y,
            layer.frame.size.width * percent_completed,
            layer.frame.size.height,
            ProgressColor,
            CornerRadius,
            true
        );
	}

    void UIProgressBar::Update()
    {
        ProgressColor.alpha = layer.color.alpha;

        if (Value != m_PreviousValue)
            for (auto& cb : m_ValueChangedCallbacks)
                cb(Value, this);

        m_PreviousValue = Value;
    }
}
