#include "OSXRenderTarget.h"
#include "OSXNativeCore.h"

namespace mc
{
	Ref<RenderTarget> mc::RenderTarget::Create(void* native)
	{
		return Ref<RenderTarget>(new OSXRenderTarget(native));
	}

	OSXRenderTarget::OSXRenderTarget(void* native)
	{
		m_NativeObject = reinterpret_cast<OSXCocoaNativeObject*>(native);
	}

	OSXRenderTarget::~OSXRenderTarget()
	{
	}

	void OSXRenderTarget::BeginDraw()
	{
	}

	void OSXRenderTarget::EndDraw()
	{
	}

	void OSXRenderTarget::ClearScreen(uint32_t r, uint32_t g, uint32_t b)
	{
		[[NSColor colorWithCalibratedRed:(float)r / 255.0f 
									green:(float)g / 255.0f 
									blue:(float)b / 255.0f 
									alpha:1.0f] 
									set];

    	NSRectFill([(OSXWindowContentViewDelegate*)m_NativeObject->contentView bounds]);
	}

	void OSXRenderTarget::PushLayer(float x, float y, float width, float height)
	{
		[[NSGraphicsContext currentContext] saveGraphicsState];

		// Create the path and add the shapes
		NSRect bounds = ((OSXWindowContentViewDelegate*)m_NativeObject->contentView).bounds;
		NSBezierPath* clipPath = [NSBezierPath bezierPath];
		[clipPath appendBezierPathWithRect:NSMakeRect(x, y, width, height)];
		[clipPath addClip];
	}

	void OSXRenderTarget::PopLayer()
	{
		[[NSGraphicsContext currentContext] restoreGraphicsState];
	}

	void OSXRenderTarget::Resize(void* native)
	{
		
	}
}
