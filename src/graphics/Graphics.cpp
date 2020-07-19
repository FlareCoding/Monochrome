#include "Graphics.h"
#include "RenderTarget.h"
#include <window/SceneManager.h>

static std::wstring ConvertStringToWstring(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}

	int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.length(), NULL, 0);
	std::wstring wstrTo;
	if (num_chars)
	{
		wstrTo.resize(num_chars);
		if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.length(), &wstrTo[0], num_chars))
		{
			return wstrTo;
		}
	}
	return std::wstring();
}

namespace mc
{

#pragma warning( push )
#pragma warning( disable : 26812 )
	static std::pair<DWRITE_TEXT_ALIGNMENT, DWRITE_PARAGRAPH_ALIGNMENT> TextAllignmentToDwriteAlignment(TextAlignment alignment)
	{
		switch (alignment)
		{
		case TextAlignment::CENTERED:
		{
			return { DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER };
		}
		case TextAlignment::LEADING:
		{
			return { DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER };
		}
		case TextAlignment::TRAILING:
		{
			return { DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER };
		}
		}

		return { DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER };
	}

	static std::pair<DWRITE_FONT_WEIGHT, DWRITE_FONT_STYLE> TextStyleToDwriteStyle(TextStyle style)
	{
		switch (style)
		{
		case TextStyle::Light:
		{
			return { DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL };
		}
		case TextStyle::ITALIC:
		{
			return { DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_ITALIC };
		}
		case TextStyle::BOLD:
		{
			return { DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL };
		}
		case TextStyle::BOLD_ITALIC:
		{
			return { DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_ITALIC };
		}
		case TextStyle::SEMIBOLD:
		{
			return { DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL };
		}
		case TextStyle::SEMIBOLD_ITALIC:
		{
			return { DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_ITALIC };
		}
		default:
			return { DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL };
		}
	}

#pragma warning( pop ) 

	static bool s_AreGraphicsInitialized = false;
	Ref<RenderTarget> Graphics::m_RenderTarget = nullptr;

	bool Graphics::Initialize(HWND hwnd)
	{
		if (!s_AreGraphicsInitialized)
		{
			m_RenderTarget = std::make_shared<RenderTarget>(hwnd);
			s_AreGraphicsInitialized = true;
		}
		else
			return false;

		m_RenderTarget->Resize(hwnd);

		return true;
	}

	bool Graphics::IsInitialized()
	{
		return s_AreGraphicsInitialized;
	}
	
	void Graphics::BeginFrame()
	{
		m_RenderTarget->BeginDraw();
	}
	
	void Graphics::EndFrame()
	{
		m_RenderTarget->EndDraw();
	}
	
	void Graphics::ClearScreenColor(uint32_t r, uint32_t g, uint32_t b)
	{
		m_RenderTarget->ClearScreen(r, g, b);
	}
	
	void Graphics::PushLayer(float x, float y, float width, float height)
	{
		m_RenderTarget->PushLayer(x, y, width, height);
	}
	
	void Graphics::PopLayer()
	{
		m_RenderTarget->PopLayer();
	}

	void Graphics::ResizeRenderTarget(HWND hwnd)
	{
		m_RenderTarget->Resize(hwnd);
	}

#pragma warning( push )
#pragma warning( disable : 6387 )
	void Graphics::DrawLine(
		float x1,
		float y1,
		float x2,
		float y2,
		Color color,
		float stroke)
	{
		ID2D1HwndRenderTarget* target = m_RenderTarget->GetNativeHandle();
		ID2D1SolidColorBrush* brush;

		target->CreateSolidColorBrush(D2D1::ColorF((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, color.alpha), &brush);

		target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), brush, stroke);

		brush->Release();
	}

	void Graphics::DrawRectangle(
		float x,
		float y,
		float width,
		float height,
		Color color,
		float corner_radius,
		bool filled,
		float stroke)
	{
		ID2D1HwndRenderTarget* target = m_RenderTarget->GetNativeHandle();

		ID2D1SolidColorBrush* brush;
		target->CreateSolidColorBrush(D2D1::ColorF((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, color.alpha), &brush);

		D2D1_RECT_F rectangle = D2D1::RectF(x, y, x + width, y + height);

		if (corner_radius > 0)
		{
			D2D1_ROUNDED_RECT rounded_rectangle = D2D1::RoundedRect(rectangle, corner_radius, corner_radius);

			if (filled)
				target->FillRoundedRectangle(rounded_rectangle, brush);
			else
				target->DrawRoundedRectangle(rounded_rectangle, brush, stroke);
		}
		else
		{
			if (filled)
				target->FillRectangle(rectangle, brush);
			else
				target->DrawRectangle(rectangle, brush, stroke);
		}

		brush->Release();
	}

	void Graphics::DrawCircle(
		float x,
		float y,
		float radius,
		Color color,
		bool filled,
		float stroke)
	{
		ID2D1HwndRenderTarget* target = m_RenderTarget->GetNativeHandle();

		ID2D1SolidColorBrush* brush;
		target->CreateSolidColorBrush(D2D1::ColorF((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, color.alpha), &brush);

		if (filled)
			target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush);
		else
			target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), brush, stroke);

		brush->Release();
	}

	void Graphics::DrawArc(
		float start_x,
		float start_y,
		float end_x,
		float end_y,
		float size,
		Color color,
		bool clockwise,
		bool large_arc,
		float stroke)
	{
		// TO-DO
	}

	void Graphics::DrawTextWideString(
		float x,
		float y,
		float width,
		float height,
		const std::wstring& text,
		TextProperties text_props,
		Color color)
	{
		ID2D1HwndRenderTarget* target = m_RenderTarget->GetNativeHandle();

		ID2D1SolidColorBrush* brush;
		target->CreateSolidColorBrush(D2D1::ColorF((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, color.alpha), &brush);

		auto [font_weight, font_style] = TextStyleToDwriteStyle(text_props.Style);

		IDWriteTextFormat* format;
		HRESULT result = CoreResources::GetWriteFactory()->CreateTextFormat(
			std::wstring(text_props.Font.begin(), text_props.Font.end()).c_str(),
			NULL,
			font_weight,
			font_style,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)text_props.FontSize,
			L"",
			&format
		);

		auto [ta, pa] = TextAllignmentToDwriteAlignment(text_props.Allignment);
		format->SetTextAlignment(ta);
		format->SetParagraphAlignment(pa);
		target->DrawTextW(text.c_str(), (UINT32)text.size(), format, D2D1::RectF(x, y, x + width, y + height), brush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		brush->Release();
		format->Release();
	}

	void Graphics::DrawTextString(
		float x,
		float y,
		float width,
		float height,
		const std::string& text,
		TextProperties text_props,
		Color color)
	{
		DrawTextWideString(x, y, width, height, ConvertStringToWstring(text), text_props, color);
	}

#pragma warning( pop ) 

	void Graphics::Update(const Color& background, SceneManager& sm)
	{
		BeginFrame();
		ClearScreenColor(background.r, background.g, background.b);

		sm.RenderGraphics();

		EndFrame();
	}
}
