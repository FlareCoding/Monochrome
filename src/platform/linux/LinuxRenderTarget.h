#pragma once
#include <graphics/RenderTarget.h>
#include <X11/Xlib.h>
#include <cairo.h>
#include <cairo-xlib.h>

namespace mc
{
	class LinuxRenderTarget : public RenderTarget
	{
	public:
		LinuxRenderTarget(void* init_info);
		~LinuxRenderTarget();

		// Inherited via RenderTarget
		virtual void* const GetNativeHandle() const override { return m_Context; }
		virtual void BeginDraw() override;
		virtual void EndDraw() override;
		virtual void ClearScreen(uint32_t r, uint32_t g, uint32_t b) override;
		virtual void PushLayer(float x, float y, float width, float height) override;
		virtual void PopLayer() override;
		virtual void Resize(void* native) override;

	private:
		cairo_surface_t* m_Surface = nullptr;
		cairo_t* m_Context = nullptr;
		int m_Width = 0, m_Height = 0;
	};
}
