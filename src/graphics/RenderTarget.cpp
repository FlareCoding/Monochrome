#include "RenderTarget.h"

namespace mc
{
	RenderTarget::RenderTarget(HWND hwnd)
	{
		if (!CoreResources::IsInitialized()) CoreResources::Initialize();

		m_NativeHandle = CoreResources::CreateRenderTarget(hwnd);
	}

	void RenderTarget::BeginDraw()
	{
		m_NativeHandle->BeginDraw();
	}

	void RenderTarget::EndDraw()
	{
		m_NativeHandle->EndDraw();
	}

	void RenderTarget::ClearScreen(uint32_t r, uint32_t g, uint32_t b)
	{
		m_NativeHandle->Clear(D2D1::ColorF((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f));
	}

	void RenderTarget::PushLayer(float x, float y, float width, float height)
	{
		D2D1_RECT_F bounds = D2D1::RectF(x, y, x + width, y + height);
		m_NativeHandle->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	void RenderTarget::PopLayer()
	{
		m_NativeHandle->PopAxisAlignedClip();
	}

	void RenderTarget::Resize(HWND hwnd)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);

		float window_dpi = (float)GetDpiForWindow(hwnd);
		UINT width = (UINT)((float)(rect.right - rect.left) * window_dpi / 96.0f);
		UINT height = (UINT)((float)(rect.bottom - rect.top) * window_dpi / 96.0f);

		m_NativeHandle->Resize(D2D1::SizeU(width, height));
	}
}
