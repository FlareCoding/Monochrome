#pragma once
#include <cinttypes>
#include "d2d_resources/CoreResources.h"

#include <wrl/client.h>
using namespace Microsoft::WRL;

struct ID2D1HwndRenderTarget;

namespace mc
{
	class RenderTarget
	{
	public:
		RenderTarget(HWND hwnd);

		inline ID2D1HwndRenderTarget* const GetNativeHandle() const { return m_NativeHandle.Get(); }

		void BeginDraw();
		void EndDraw();
		void ClearScreen(uint32_t r, uint32_t g, uint32_t b);

		void PushLayer(float x, float y, float width, float height);
		void PopLayer();

		void Resize(HWND hwnd);

		~RenderTarget() = default;

	private:
		ComPtr<ID2D1HwndRenderTarget> m_NativeHandle;
	};
}
