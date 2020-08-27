#pragma once
#include <graphics/RenderTarget.h>

struct OSXCocoaNativeObject;

namespace mc
{
	class OSXRenderTarget : public RenderTarget
	{
	public:
		OSXRenderTarget(void* native);
		~OSXRenderTarget();

		// Inherited via RenderTarget
		virtual void* const GetNativeHandle() const override { return m_NativeObject; }
		virtual void BeginDraw() override;
		virtual void EndDraw() override;
		virtual void ClearScreen(uint32_t r, uint32_t g, uint32_t b) override;
		virtual void PushLayer(float x, float y, float width, float height) override;
		virtual void PopLayer() override;
		virtual void Resize(void* native) override;

	private:
		OSXCocoaNativeObject* m_NativeObject = nullptr;
	};
}
