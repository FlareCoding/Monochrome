#include "WindowsRenderTarget.h"

namespace mc
{
	Ref<RenderTarget> mc::RenderTarget::Create(void* native)
	{
		return Ref<RenderTarget>(new WindowsRenderTarget((HWND)native));
	}

	WindowsRenderTarget::WindowsRenderTarget(HWND hwnd)
	{
		if (!CoreResources::IsInitialized()) CoreResources::Initialize();

		m_NativeHandle = CoreResources::CreateRenderTarget(hwnd);
	}

	void WindowsRenderTarget::BeginDraw()
	{
		m_NativeHandle->BeginDraw();
	}

	void WindowsRenderTarget::EndDraw()
	{
		m_NativeHandle->EndDraw();
	}

	void WindowsRenderTarget::ClearScreen(uint32_t r, uint32_t g, uint32_t b)
	{
		m_NativeHandle->Clear(D2D1::ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f));
	}

	void WindowsRenderTarget::PushLayer(float x, float y, float width, float height)
	{
		D2D1_RECT_F bounds = D2D1::RectF(x, y, x + width, y + height);
		m_NativeHandle->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	void WindowsRenderTarget::PopLayer()
	{
		m_NativeHandle->PopAxisAlignedClip();
	}

	void WindowsRenderTarget::Resize(void* hwnd)
	{
		RECT rect;
		GetClientRect((HWND)hwnd, &rect);

		float window_dpi = (float)GetDpiForWindow((HWND)hwnd);
		UINT width = (UINT)((float)(rect.right - rect.left) * window_dpi / 96.0f);
		UINT height = (UINT)((float)(rect.bottom - rect.top) * window_dpi / 96.0f);

		m_NativeHandle->Resize(D2D1::SizeU(width, height));
	}
}
