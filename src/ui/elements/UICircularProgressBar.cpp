#include "UICircularProgressBar.h"
#include <graphics/Graphics.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <math.h>
#endif

namespace mc
{
    UICircularProgressBar::UICircularProgressBar()
    {
        layer.frame = Frame(60, 120, 80, 80);
        SetDefaultOptions();
    }

    UICircularProgressBar::UICircularProgressBar(Frame frame) : UIView(frame)
    {
        SetDefaultOptions();
    }

    void UICircularProgressBar::SetDefaultOptions()
    {
        CornerRadius = 2.0f;
        layer.color = Color::white;
        Stroke = 4.0f;
    }

    void UICircularProgressBar::Draw()
    {
        Update();

        Graphics::DrawCircle(
            layer.frame.position.x + layer.frame.size.width  / 2.0f,
            layer.frame.position.y + layer.frame.size.height / 2.0f,
            m_Radius,
            layer.color,
            false,
            Stroke
        );
        
        Graphics::DrawArc(
            layer.frame.position.x + layer.frame.size.width / 2.0f,
            layer.frame.position.y + Stroke * 2.0f,
            m_ArcEndPoint.x,
            m_ArcEndPoint.y,
            m_Angle,
            m_Radius,
            ProgressColor,
            true,
            m_IsLargeArc,
            Stroke
        );
    }

    void UICircularProgressBar::Update()
    {
        ProgressColor.alpha = layer.color.alpha;

        if (Value != m_PreviousValue)
            for (auto& cb : m_ValueChangedCallbacks)
                cb(Value, this);

        m_PreviousValue = Value;

        m_Radius = layer.frame.size.width / 2.0f - Stroke * 2.0f;

        m_PercentCompleted = Value / MaxValue;
        if (m_PercentCompleted < 0)
            m_PercentCompleted = 0;
        else if (m_PercentCompleted > 1)
            m_PercentCompleted = 1;

        if (m_PercentCompleted < 0.5f)
            m_IsLargeArc = false;
        else
            m_IsLargeArc = true;

        m_Angle = 2 * 3.14f * m_PercentCompleted;

        float x_coord = layer.frame.position.x + sinf(m_Angle) * m_Radius + layer.frame.size.width / 2.0f;
        float y_coord = layer.frame.position.y - cosf(m_Angle) * m_Radius + layer.frame.size.height / 2.0f;

        m_ArcEndPoint = { x_coord, y_coord };
    }
}
