#include "LinuxRenderTarget.h"

namespace mc
{
	typedef struct _LinuxGraphicsNativeInformation
	{
		Display*    display;
		int         screen;
		Visual*     visual;
		Window      window;
		int 		window_width;
		int			window_height;
	} LinuxGraphicsNativeInformation;

	Ref<RenderTarget> mc::RenderTarget::Create(void* native)
	{
		return Ref<RenderTarget>(new LinuxRenderTarget(native));
	}

	LinuxRenderTarget::LinuxRenderTarget(void* init_info)
	{
		LinuxGraphicsNativeInformation* inf = reinterpret_cast<LinuxGraphicsNativeInformation*>(init_info);

		m_Surface = cairo_xlib_surface_create(inf->display, inf->window, inf->visual, inf->window_width, inf->window_height);
	    cairo_xlib_surface_set_size(m_Surface, inf->window_width, inf->window_height);
	    m_Context = cairo_create(m_Surface);

		m_Width = inf->window_width;
		m_Height = inf->window_height;
	}

	LinuxRenderTarget::~LinuxRenderTarget()
	{
		if (m_Context) cairo_destroy(m_Context);
	}

	void LinuxRenderTarget::BeginDraw()
	{
	}

	void LinuxRenderTarget::EndDraw()
	{
	}

	void LinuxRenderTarget::ClearScreen(uint32_t r, uint32_t g, uint32_t b)
	{
		cairo_set_source_rgb(m_Context, (double)r / 255, (double)g / 255, (double)b / 255);
	    cairo_rectangle(m_Context, 0, 0, m_Width, m_Height);
	    cairo_fill(m_Context);
	}

	void LinuxRenderTarget::PushLayer(float x, float y, float width, float height)
	{
		cairo_save(m_Context);
		cairo_rectangle(m_Context, x, y, width, height);
		cairo_clip(m_Context);
	}

	void LinuxRenderTarget::PopLayer()
	{
		cairo_restore(m_Context);
	}

	void LinuxRenderTarget::Resize(void* native)
	{
		LinuxGraphicsNativeInformation* inf = reinterpret_cast<LinuxGraphicsNativeInformation*>(native);

		m_Surface = cairo_xlib_surface_create(inf->display, inf->window, inf->visual, inf->window_width, inf->window_height);
	 	cairo_xlib_surface_set_size(m_Surface, inf->window_width, inf->window_height);
		
		cairo_set_source_surface(m_Context, m_Surface, 0, 0);
	}
}
