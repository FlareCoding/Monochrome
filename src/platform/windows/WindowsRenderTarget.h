#pragma once
#include <graphics/RenderTarget.h>
#include "d2d_resources/CoreResources.h"

#include <wrl/client.h>
using namespace Microsoft::WRL;

struct ID2D1HwndRenderTarget;

namespace mc
{
	class WindowsRenderTarget : public RenderTarget
	{
	public:
		WindowsRenderTarget(HWND hwnd);

		// Inherited via RenderTarget
		virtual void* const GetNativeHandle() const override { return m_NativeHandle.Get(); }
		virtual void BeginDraw() override;
		virtual void EndDraw() override;
		virtual void ClearScreen(uint32_t r, uint32_t g, uint32_t b) override;
		virtual void PushLayer(float x, float y, float width, float height) override;
		virtual void PopLayer() override;
		virtual void Resize(void* native) override;

	private:
		ComPtr<ID2D1HwndRenderTarget> m_NativeHandle;
	};
}
