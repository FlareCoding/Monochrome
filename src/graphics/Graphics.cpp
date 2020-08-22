#include "Graphics.h"
#include <window/SceneManager.h>

#if defined(_WIN32)
#include <platform/WindowsGraphics.h>
#elif defined(__linux__)
#error Monochrome is not yet supported on Linux
#elif defined(__APPLE__)
#error Monochrome is not yet supported on OS X
#else
#error Unknown platform
#endif

namespace mc
{
	bool Graphics::Initialize(void* native)
	{
#if defined(_WIN32)
		return WindowsGraphics::Initialize(reinterpret_cast<HWND>(native));
#elif defined(__linux__)
	
#endif
	}

	bool Graphics::IsInitialized()
	{
#if defined(_WIN32)
		return WindowsGraphics::IsInitialized();
#elif defined(__linux__)

#endif	
	}

	void Graphics::Shutdown()
	{
#if defined(_WIN32)
		WindowsGraphics::Shutdown();
#elif defined(__linux__)

#endif
	}

	void Graphics::SetActiveTarget(void* native)
	{
#if defined(_WIN32)
		WindowsGraphics::SetActiveTarget(reinterpret_cast<HWND>(native));
#elif defined(__linux__)

#endif
	}
	
	void Graphics::BeginFrame()
	{
#if defined(_WIN32)
		WindowsGraphics::BeginFrame();
#elif defined(__linux__)

#endif
	}
	
	void Graphics::EndFrame()
	{
#if defined(_WIN32)
		WindowsGraphics::EndFrame();
#elif defined(__linux__)

#endif
	}
	
	void Graphics::ClearScreenColor(uint32_t r, uint32_t g, uint32_t b)
	{
#if defined(_WIN32)
		WindowsGraphics::ClearScreenColor(r, g, b);
#elif defined(__linux__)

#endif
	}
	
	void Graphics::PushLayer(float x, float y, float width, float height)
	{
#if defined(_WIN32)
		WindowsGraphics::PushLayer(x, y, width, height);
#elif defined(__linux__)

#endif
	}
	
	void Graphics::PopLayer()
	{
#if defined(_WIN32)
		WindowsGraphics::PopLayer();
#elif defined(__linux__)

#endif
	}

	void Graphics::ResizeRenderTarget(void* native)
	{
#if defined(_WIN32)
		WindowsGraphics::ResizeRenderTarget(reinterpret_cast<HWND>(native));
#elif defined(__linux__)

#endif
	}

	void Graphics::DrawLine(
		float x1,
		float y1,
		float x2,
		float y2,
		Color color,
		float stroke)
	{
#if defined(_WIN32)
		WindowsGraphics::DrawLine(x1, y1, x2, y2, color, stroke);
#elif defined(__linux__)

#endif	
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
#if defined(_WIN32)
		WindowsGraphics::DrawRectangle(x, y, width, height, color, corner_radius, filled, stroke);
#elif defined(__linux__)

#endif
	}

	void Graphics::DrawCircle(
		float x,
		float y,
		float radius,
		Color color,
		bool filled,
		float stroke)
	{
#if defined(_WIN32)
		WindowsGraphics::DrawCircle(x, y, radius, color, filled, stroke);
#elif defined(__linux__)

#endif
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
#if defined(_WIN32)
		WindowsGraphics::DrawArc(start_x, start_y, end_x, end_y, size, color, clockwise, large_arc, stroke);
#elif defined(__linux__)

#endif
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
#if defined(_WIN32)
		WindowsGraphics::DrawTextWideString(x, y, width, height, text, text_props, color);
#elif defined(__linux__)

#endif
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
#if defined(_WIN32)
		WindowsGraphics::DrawTextString(x, y, width, height, text, text_props, color);
#elif defined(__linux__)

#endif
	}

	TextMetrics Graphics::CalculateTextMetrics(
		const std::string& text,
		TextProperties text_props,
		float max_width,
		float max_height)
	{
#if defined(_WIN32)
		return WindowsGraphics::CalculateTextMetrics(text, text_props, max_width, max_height);
#elif defined(__linux__)

#endif
	}

	Ref<Bitmap> Graphics::CreateBitmapFromFile(const std::string& path)
	{
#if defined(_WIN32)
		return WindowsGraphics::CreateBitmapFromFile(path);
#elif defined(__linux__)

#endif
	}

	Ref<Bitmap> Graphics::CreateBitmap(const char* data, uint32_t size)
	{
#if defined(_WIN32)
		return WindowsGraphics::CreateBitmap(data, size);
#elif defined(__linux__)

#endif
	}

	void Graphics::DrawBitmapImage(
		Ref<Bitmap>& bmp,
		float x,
		float y,
		float width,
		float height,
		float opacity
	)
	{
#if defined(_WIN32)
		WindowsGraphics::DrawBitmapImage(bmp, x, y, width, height, opacity);
#elif defined(__linux__)

#endif
	}

	void Graphics::Update(const Color& background, SceneManager& sm, bool clearBackgroundColor)
	{
#if defined(_WIN32)
		WindowsGraphics::Update(background, sm, clearBackgroundColor);
#elif defined(__linux__)

#endif
	}
}
