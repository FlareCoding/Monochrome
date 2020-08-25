#pragma once
#include <core/Core.h>
#include <cinttypes>

namespace mc
{
	class RenderTarget
	{
	public:
		static Ref<RenderTarget> Create(void* native);

		virtual ~RenderTarget() = default;

		virtual void* const GetNativeHandle() const = 0;

		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;
		virtual void ClearScreen(uint32_t r, uint32_t g, uint32_t b) = 0;

		virtual void PushLayer(float x, float y, float width, float height) = 0;
		virtual void PopLayer() = 0;

		virtual void Resize(void* native) = 0;
	};
}
