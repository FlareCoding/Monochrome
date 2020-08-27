#include "OSXGraphics.h"
#include <window/SceneManager.h>
#include "OSXNativeCore.h"

@interface ImageUtil : NSObject
+(NSImage*)flipImage:(NSImage*)image;
@end

@implementation ImageUtil
+(NSImage*)flipImage:(NSImage*)image
{
    NSImage *existingImage = image;
    NSSize existingSize = [existingImage size];
    NSSize newSize = NSMakeSize(existingSize.width, existingSize.height);
    NSImage *flipedImage = [[[NSImage alloc] initWithSize:newSize] autorelease];

    [flipedImage lockFocus];
    [[NSGraphicsContext currentContext] setImageInterpolation:NSImageInterpolationHigh];

    NSAffineTransform *t = [NSAffineTransform transform];
    [t translateXBy:existingSize.width yBy:existingSize.height];
    [t scaleXBy:-1 yBy:-1];
    [t concat];

    [existingImage drawAtPoint:NSZeroPoint fromRect:NSMakeRect(0, 0, newSize.width, newSize.height) operation:NSCompositingOperationSourceOver fraction:1.0];

    [flipedImage unlockFocus];

    return flipedImage;
}
@end

namespace mc
{
    static bool s_AreOSXGraphicsInitialized = false;
    Ref<RenderTarget> OSXGraphics::m_RenderTarget = nullptr;

    bool OSXGraphics::Initialize(void* native)
	{
        m_RenderTarget = RenderTarget::Create(native);
		m_RenderTarget->Resize(native);

        s_AreOSXGraphicsInitialized = true;
		return true;
	}

    bool OSXGraphics::IsInitialized()
	{
		return s_AreOSXGraphicsInitialized;
	}

    void OSXGraphics::Shutdown()
	{
	}

	void OSXGraphics::SetActiveTarget(void* native)
	{
	}

    void OSXGraphics::BeginFrame()
	{
		m_RenderTarget->BeginDraw();
	}

	void OSXGraphics::EndFrame()
	{
		m_RenderTarget->EndDraw();
	}

	void OSXGraphics::ClearScreenColor(uint32_t r, uint32_t g, uint32_t b)
	{
		m_RenderTarget->ClearScreen(r, g, b);
	}

    void OSXGraphics::PushLayer(float x, float y, float width, float height)
	{
		m_RenderTarget->PushLayer(x, y, width, height);
	}

	void OSXGraphics::PopLayer()
	{
		m_RenderTarget->PopLayer();
	}

	void OSXGraphics::ResizeRenderTarget(void* native)
	{
		m_RenderTarget->Resize(native);
	}

#pragma warning( push )
#pragma warning( disable : 6387 )
	void OSXGraphics::DrawLine(
		float x1,
		float y1,
		float x2,
		float y2,
		Color color,
		float stroke)
	{
		[[NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:1.0f] 
									set];

		NSBezierPath* path = [NSBezierPath bezierPath];
		[path moveToPoint:NSMakePoint(x1, y1)];
		[path lineToPoint:NSMakePoint(x2, y2)];
		[path setLineWidth:stroke];
		[path stroke];
	}

	void OSXGraphics::DrawRectangle(
		float x,
		float y,
		float width,
		float height,
		Color color,
		float radius,
		bool filled,
		float stroke)
	{
		[[NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:1.0f] 
									set];

		NSBezierPath* path = [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(x, y, width, height)
                                  xRadius:radius
                                  yRadius:radius];
		
		if (filled)
		{
			[path fill];
		}
		else
		{
			[path setLineWidth:stroke];
			[path stroke];
		}
	}

	void OSXGraphics::DrawCircle(
		float x,
		float y,
		float radius,
		Color color,
		bool filled,
		float stroke)
	{
		[[NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:1.0f] 
									set];

		NSRect rect = NSMakeRect(x - radius, y - radius, radius * 2, radius * 2);
		NSBezierPath* path = [NSBezierPath bezierPathWithOvalInRect:rect];
		if (filled)
		{
			[path fill];
		}
		else
		{
			[path setLineWidth:stroke];
			[path stroke];
		}
	}

	void OSXGraphics::DrawArc(
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
		[[NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:1.0f] 
									setFill];

		// TO-DO
	}

	void OSXGraphics::DrawTextWideString(
		float x,
		float y,
		float width,
		float height,
		const std::wstring& text,
		TextProperties text_props,
		Color color)
	{
		NSColor* textColor = [NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:color.alpha];

		NSFont* font = [NSFont fontWithName:[NSString stringWithUTF8String: text_props.Font.c_str()] size:text_props.FontSize];
		NSDictionary* attribs = @{ 
									NSForegroundColorAttributeName : textColor,
									NSFontAttributeName : font,
								};

		NSString* str = [[NSString alloc] initWithBytes:text.c_str() length:text.size() * sizeof(wchar_t) encoding:NSUTF32LittleEndianStringEncoding];
		NSAttributedString* attributedString = [[NSAttributedString alloc] initWithString:str attributes:attribs];

		NSRect layerRect = NSMakeRect(x, y, width, height);
		CGSize stringSize = [str sizeWithAttributes:attribs];

		float x_pos = 0;
		if (text_props.Allignment == TextAlignment::CENTERED)
			x_pos = layerRect.origin.x + (layerRect.size.width - stringSize.width) / 2; 
		else if (text_props.Allignment == TextAlignment::LEADING)
			x_pos = layerRect.origin.x; 
		else if (text_props.Allignment == TextAlignment::TRAILING)
			x_pos = layerRect.origin.x + layerRect.size.width - stringSize.width;

		float y_pos = (layerRect.size.height - stringSize.height) / 2;
		[str drawAtPoint:CGPointMake(x_pos, layerRect.origin.y + y_pos) withAttributes:attribs];
	}

	void OSXGraphics::DrawTextString(
		float x,
		float y,
		float width,
		float height,
		const std::string& text,
		TextProperties text_props,
		Color color)
	{
		DrawTextWideString(x, y, width, height, std::wstring(text.begin(), text.end()), text_props, color);
	}

	TextMetrics OSXGraphics::CalculateTextMetrics(
		const std::string& text,
		TextProperties text_props,
		float max_width,
		float max_height)
	{
		TextMetrics metrics = { 0 };
		static float static_text_metric_offset = 5.0f;
		
		NSString* fontName = [NSString stringWithUTF8String:text_props.Font.c_str()];
		NSFont* font = [NSFont fontWithName:fontName size:text_props.FontSize];
		NSDictionary* attribs = @{ 
									NSFontAttributeName : font,
								};

		NSString* str = [NSString stringWithUTF8String:text.c_str()];
		NSAttributedString* attributedString = [[NSAttributedString alloc] initWithString:str attributes:attribs];

		NSRect layerRect = NSMakeRect(0, 0, max_width, max_height);
		CGSize stringSize = [str sizeWithAttributes:attribs];

		metrics.Width = stringSize.width + static_text_metric_offset;
		metrics.WidthIncludingTrailingWhitespace = stringSize.width + static_text_metric_offset;
		metrics.Height = stringSize.height;
		metrics.LineCount = 1;

		return metrics;
	}

	uint32_t OSXGraphics::GetLineCharacterLimit(TextProperties text_props, float container_width, float container_height)
	{
		return 0;
	}

	Ref<Bitmap> OSXGraphics::CreateBitmapFromFile(const std::string& path)
	{
        NSImage* img = [NSImage imageNamed:[NSString stringWithUTF8String:path.c_str()]];
		return MakeRef<Bitmap>(reinterpret_cast<void*>([ImageUtil flipImage:img]));
	}

	Ref<Bitmap> OSXGraphics::CreateBitmapFromURL(const char* url)
	{
		NSURL* imageURL = [NSURL URLWithString:[NSString stringWithUTF8String:url]];
		NSImage* img = [[NSImage alloc] initWithContentsOfURL:imageURL];

		return MakeRef<Bitmap>(reinterpret_cast<void*>([ImageUtil flipImage:img]));
	}

	void OSXGraphics::DrawBitmapImage(
		Ref<Bitmap>& bmp,
		float x,
		float y,
		float width,
		float height,
		float opacity
	)
	{
		NSImage* img = reinterpret_cast<NSImage*>(bmp->GetBmpData());

		NSRect rect = NSMakeRect(x, y, width, height);
		[img drawInRect:rect fromRect:NSZeroRect operation:NSCompositingOperationSourceOver fraction:1.0f];
	}

    void OSXGraphics::Update(const Color& background, SceneManager& sm, bool clearBackgroundColor)
	{
		BeginFrame();
		if (clearBackgroundColor) ClearScreenColor(background.r, background.g, background.b);

		sm.RenderGraphics();

		EndFrame();
	}
}
