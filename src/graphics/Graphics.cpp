#include "Graphics.h"
#include <window/SceneManager.h>

#if defined(_WIN32)
#include <platform/windows/WindowsGraphics.h>
#elif defined(__linux__)
#include <platform/linux/LinuxGraphics.h>
#elif defined(__APPLE__)
#include <platform/osx/OSXGraphics.h>
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
		return LinuxGraphics::Initialize(native);
#elif defined(__APPLE__)
		return OSXGraphics::Initialize(native);
#endif
	}

	bool Graphics::IsInitialized()
	{
#if defined(_WIN32)
		return WindowsGraphics::IsInitialized();
#elif defined(__linux__)
		return LinuxGraphics::IsInitialized();
#elif defined(__APPLE__)
		return OSXGraphics::IsInitialized();
#endif
	}

	void Graphics::Shutdown()
	{
#if defined(_WIN32)
		WindowsGraphics::Shutdown();
#elif defined(__linux__)
		LinuxGraphics::Shutdown();
	#elif defined(__APPLE__)
		OSXGraphics::Shutdown();
#endif
	}

	void Graphics::SetActiveTarget(void* native)
	{
#if defined(_WIN32)
		WindowsGraphics::SetActiveTarget(reinterpret_cast<HWND>(native));
#elif defined(__linux__)
		LinuxGraphics::SetActiveTarget(native);
#elif defined(__APPLE__)
		OSXGraphics::SetActiveTarget(native);
#endif
	}

	void Graphics::BeginFrame()
	{
#if defined(_WIN32)
		WindowsGraphics::BeginFrame();
#elif defined(__linux__)
		LinuxGraphics::BeginFrame();
#elif defined(__APPLE__)
		OSXGraphics::BeginFrame();
#endif
	}

	void Graphics::EndFrame()
	{
#if defined(_WIN32)
		WindowsGraphics::EndFrame();
#elif defined(__linux__)
		LinuxGraphics::EndFrame();
#elif defined(__APPLE__)
		OSXGraphics::EndFrame();
#endif
	}

	void Graphics::ClearScreenColor(uint32_t r, uint32_t g, uint32_t b)
	{
#if defined(_WIN32)
		WindowsGraphics::ClearScreenColor(r, g, b);
#elif defined(__linux__)
		LinuxGraphics::ClearScreenColor(r, g, b);
#elif defined(__APPLE__)
		OSXGraphics::ClearScreenColor(r, g, b);
#endif
	}

	void Graphics::PushLayer(float x, float y, float width, float height)
	{
#if defined(_WIN32)
		WindowsGraphics::PushLayer(x, y, width, height);
#elif defined(__linux__)
		LinuxGraphics::PushLayer(x, y, width, height);
#elif defined(__APPLE__)
		OSXGraphics::PushLayer(x, y, width, height);
#endif
	}

	void Graphics::PopLayer()
	{
#if defined(_WIN32)
		WindowsGraphics::PopLayer();
#elif defined(__linux__)
		LinuxGraphics::PopLayer();
#elif defined(__APPLE__)
		OSXGraphics::PopLayer();
#endif
	}

	void Graphics::ResizeRenderTarget(void* native)
	{
#if defined(_WIN32)
		WindowsGraphics::ResizeRenderTarget(reinterpret_cast<HWND>(native));
#elif defined(__linux__)
		LinuxGraphics::ResizeRenderTarget(native);
#elif defined(__APPLE__)
		OSXGraphics::ResizeRenderTarget(native);
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
		LinuxGraphics::DrawLine(x1, y1, x2, y2, color, stroke);
#elif defined(__APPLE__)
		OSXGraphics::DrawLine(x1, y1, x2, y2, color, stroke);
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
		LinuxGraphics::DrawRectangle(x, y, width, height, color, corner_radius, filled, stroke);
#elif defined(__APPLE__)
		OSXGraphics::DrawRectangle(x, y, width, height, color, corner_radius, filled, stroke);
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
		LinuxGraphics::DrawCircle(x, y, radius, color, filled, stroke);
#elif defined(__APPLE__)
		OSXGraphics::DrawCircle(x, y, radius, color, filled, stroke);
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
		LinuxGraphics::DrawArc(start_x, start_y, end_x, end_y, size, color, clockwise, large_arc, stroke);
#elif defined(__APPLE__)
		OSXGraphics::DrawArc(start_x, start_y, end_x, end_y, size, color, clockwise, large_arc, stroke);
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
		LinuxGraphics::DrawTextWideString(x, y, width, height, text, text_props, color);
#elif defined(__APPLE__)
		OSXGraphics::DrawTextWideString(x, y, width, height, text, text_props, color);
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
		LinuxGraphics::DrawTextString(x, y, width, height, text, text_props, color);
#elif defined(__APPLE__)
		OSXGraphics::DrawTextString(x, y, width, height, text, text_props, color);
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
		return LinuxGraphics::CalculateTextMetrics(text, text_props, max_width, max_height);
#elif defined(__APPLE__)
		return OSXGraphics::CalculateTextMetrics(text, text_props, max_width, max_height);
#endif
	}

	Ref<Bitmap> Graphics::CreateBitmapFromFile(const std::string& path)
	{
#if defined(_WIN32)
		return WindowsGraphics::CreateBitmapFromFile(path);
#elif defined(__linux__)
		return LinuxGraphics::CreateBitmapFromFile(path);
#elif defined(__APPLE__)
		return OSXGraphics::CreateBitmapFromFile(path);
#endif
	}

	Ref<Bitmap> Graphics::CreateBitmap(const char* data, uint32_t size)
	{
#if defined(_WIN32)
		return WindowsGraphics::CreateBitmap(data, size);
#elif defined(__linux__)
		return LinuxGraphics::CreateBitmap(data, size);
#elif defined(__APPLE__)
		return nullptr;
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
		LinuxGraphics::DrawBitmapImage(bmp, x, y, width, height, opacity);
#elif defined(__APPLE__)
		OSXGraphics::DrawBitmapImage(bmp, x, y, width, height, opacity);
#endif
	}

	void Graphics::Update(const Color& background, SceneManager& sm, bool clearBackgroundColor)
	{
#if defined(_WIN32)
		WindowsGraphics::Update(background, sm, clearBackgroundColor);
#elif defined(__linux__)
		LinuxGraphics::Update(background, sm, clearBackgroundColor);
#elif defined(__APPLE__)
		OSXGraphics::Update(background, sm, clearBackgroundColor);
#endif
	}
}
