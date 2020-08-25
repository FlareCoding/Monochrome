#include "LinuxGraphics.h"
#include <window/SceneManager.h>

#include <X11/Xlib.h>
#include <cairo.h>
#include <cairo-xlib.h>
#include <math.h>

namespace mc
{
    typedef struct _LinuxGraphicsNativeInformation
    {
        int         id;
        Display*    display;
        int         screen;
        Visual*     visual;
        Window      window;
        int 		window_width;
        int			window_height;
    } LinuxGraphicsNativeInformation;

    static bool s_AreLinuxGraphicsInitialized = false;
    Ref<RenderTarget> LinuxGraphics::m_RenderTarget = nullptr;
    std::map<int, Ref<RenderTarget>> LinuxGraphics::m_RenderTargetMap;

    static double DEG = 3.141592653589 / 180.0;

    bool LinuxGraphics::Initialize(void* init_info)
	{
        LinuxGraphicsNativeInformation* inf = reinterpret_cast<LinuxGraphicsNativeInformation*>(init_info);
        m_RenderTargetMap[inf->id] = RenderTarget::Create(init_info);
		m_RenderTargetMap[inf->id]->Resize(init_info);

        s_AreLinuxGraphicsInitialized = true;
		return true;
	}

    bool LinuxGraphics::IsInitialized()
	{
		return s_AreLinuxGraphicsInitialized;
	}

    void LinuxGraphics::Shutdown()
	{
	}

	void LinuxGraphics::SetActiveTarget(void* native)
	{
        LinuxGraphicsNativeInformation* inf = reinterpret_cast<LinuxGraphicsNativeInformation*>(native);
        m_RenderTarget = m_RenderTargetMap[inf->id];
	}

    void LinuxGraphics::BeginFrame()
	{
		m_RenderTarget->BeginDraw();
	}

	void LinuxGraphics::EndFrame()
	{
		m_RenderTarget->EndDraw();
	}

	void LinuxGraphics::ClearScreenColor(uint32_t r, uint32_t g, uint32_t b)
	{
		m_RenderTarget->ClearScreen(r, g, b);
	}

    void LinuxGraphics::PushLayer(float x, float y, float width, float height)
	{
		m_RenderTarget->PushLayer(x, y, width, height);
	}

	void LinuxGraphics::PopLayer()
	{
		m_RenderTarget->PopLayer();
	}

	void LinuxGraphics::ResizeRenderTarget(void* native)
	{
		m_RenderTarget->Resize(native);
	}

#pragma warning( push )
#pragma warning( disable : 6387 )
	void LinuxGraphics::DrawLine(
		float x1,
		float y1,
		float x2,
		float y2,
		Color color,
		float stroke)
	{
        auto* ctx = reinterpret_cast<cairo_t*>(m_RenderTarget->GetNativeHandle());

        cairo_set_line_width(ctx, stroke);
        cairo_move_to(ctx, x1, y1);
        cairo_line_to(ctx, x2, y2);

        cairo_set_source_rgba(ctx, (double)color.r / 255, (double)color.g / 255, (double)color.b / 255, (double)color.alpha);
        cairo_fill(ctx);
	}

	void LinuxGraphics::DrawRectangle(
		float x,
		float y,
		float width,
		float height,
		Color color,
		float radius,
		bool filled,
		float stroke)
	{
        auto* ctx = reinterpret_cast<cairo_t*>(m_RenderTarget->GetNativeHandle());

        cairo_new_sub_path (ctx);
        cairo_arc (ctx, x + width - radius, y + radius, radius, -90 * DEG, 0 * DEG);
        cairo_arc (ctx, x + width - radius, y + height - radius, radius, 0 * DEG, 90 * DEG);
        cairo_arc (ctx, x + radius, y + height - radius, radius, 90 * DEG, 180 * DEG);
        cairo_arc (ctx, x + radius, y + radius, radius, 180 * DEG, 270 * DEG);
        cairo_close_path (ctx);

        cairo_set_source_rgba(ctx, (double)color.r / 255, (double)color.g / 255, (double)color.b / 255, (double)color.alpha);
        if (filled)
            cairo_fill(ctx);
        else
            cairo_stroke(ctx);
	}

	void LinuxGraphics::DrawCircle(
		float x,
		float y,
		float radius,
		Color color,
		bool filled,
		float stroke)
	{
        auto* ctx = reinterpret_cast<cairo_t*>(m_RenderTarget->GetNativeHandle());

        cairo_arc(ctx, x, y, radius, 0, 2 * M_PI);

        cairo_set_source_rgba(ctx, (double)color.r / 255, (double)color.g / 255, (double)color.b / 255, (double)color.alpha);
        if (filled)
            cairo_fill(ctx);
        else
            cairo_stroke(ctx);
	}

	void LinuxGraphics::DrawArc(
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
	}

	void LinuxGraphics::DrawTextWideString(
		float x,
		float y,
		float width,
		float height,
		const std::wstring& text,
		TextProperties text_props,
		Color color)
	{
	}

	void LinuxGraphics::DrawTextString(
		float x,
		float y,
		float width,
		float height,
		const std::string& text,
		TextProperties text_props,
		Color color)
	{
        auto* ctx = reinterpret_cast<cairo_t*>(m_RenderTarget->GetNativeHandle());

        cairo_text_extents_t extents;
        cairo_text_extents(ctx, text.c_str(), &extents);

        cairo_set_source_rgba (ctx, (double)color.r / 255, (double)color.g / 255, (double)color.b / 255, (double)color.alpha);
        cairo_move_to(ctx, x + width / 2 - extents.width / 2 + 4, y + height / 2 + 4);
        cairo_show_text(ctx, text.c_str());
	}

	TextMetrics LinuxGraphics::CalculateTextMetrics(
		const std::string& text,
		TextProperties text_props,
		float max_width,
		float max_height)
	{
		TextMetrics metrics = { 0 };
		return metrics;
	}

	uint32_t LinuxGraphics::GetLineCharacterLimit(TextProperties text_props, float container_width, float container_height)
	{
		return 0;
	}

	Ref<Bitmap> LinuxGraphics::CreateBitmapFromFile(const std::string& path)
	{
        return nullptr;
	}

	Ref<Bitmap> LinuxGraphics::CreateBitmap(const char* data, uint32_t size)
	{
        return nullptr;
	}

	void LinuxGraphics::DrawBitmapImage(
		Ref<Bitmap>& bmp,
		float x,
		float y,
		float width,
		float height,
		float opacity
	)
	{
	}

    void LinuxGraphics::Update(const Color& background, SceneManager& sm, bool clearBackgroundColor)
	{
		BeginFrame();
		if (clearBackgroundColor) ClearScreenColor(background.r, background.g, background.b);

		sm.RenderGraphics();

		EndFrame();
	}
}
