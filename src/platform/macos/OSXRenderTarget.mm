#include "OSXRenderTarget.h"
#include <application/AppManager.h>
#include <utils/TextUtils.h>

namespace mc
{
    OSXRenderTarget::OSXRenderTarget(NSView* contentView, float dpiScalingFactor) {
        d_contentView = contentView;
        d_scalingFactor = dpiScalingFactor;
        
        // Set the application context runtime
        // function for calculating text metrics.
        auto& runtimeUtilityFunctions =
            AppManager::getApplicationContext()->getRuntimeUtilityFunctions();

        if (!runtimeUtilityFunctions.getTextPixelSizeFn) {
            runtimeUtilityFunctions.getTextPixelSizeFn =
                [this](
                    uint64_t maxWidth,
                    uint64_t maxHeight,
                    const std::string& text,
                    const std::string& font,
                    uint32_t fontSize,
                    const std::string& fontStyle,
                    const std::string& alignment,
                    const std::string& wrapMode
                ) { return runtimeCalculateTextSize(maxWidth, maxHeight, text, font, fontSize, fontStyle, alignment, wrapMode); };
        }
    }

    void OSXRenderTarget::_adjustPositionAndSizeForDPIScaling(
        int32_t& xPos,
        int32_t& yPos,
        uint32_t& width,
        uint32_t& height
    ) {
        float xPosF = static_cast<float>(xPos);
        xPos = static_cast<int32_t>(xPosF * d_scalingFactor);

        float yPosF = static_cast<float>(yPos);
        yPos = static_cast<int32_t>(yPosF * d_scalingFactor);

        float widthF = static_cast<float>(width);
        width = static_cast<uint32_t>(widthF * d_scalingFactor);

        float heightF = static_cast<float>(height);
        height = static_cast<uint32_t>(heightF * d_scalingFactor);
    }

    void OSXRenderTarget::_convertPositionToCartesianCoordinates(int32_t& yPos, uint32_t height) {
        // Get the viewport height
        float viewportHeight = static_cast<float>([d_contentView bounds].size.height) * d_scalingFactor;

        // Adjust the given height
        yPos = static_cast<int32_t>(viewportHeight) - yPos - height;
    }

    void OSXRenderTarget::resize(uint32_t width, uint32_t height) {
        d_width = width;
        d_height = height;

        d_backRenderBuffer = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
                                pixelsWide:d_width * d_scalingFactor
                                pixelsHigh:d_height * d_scalingFactor
                                bitsPerSample:8
                                samplesPerPixel:4
                                hasAlpha:YES
                                isPlanar:NO
                                colorSpaceName:NSCalibratedRGBColorSpace
                                bitmapFormat:0
                                bytesPerRow:(4 * d_width * d_scalingFactor)
                                bitsPerPixel:32];
    }

    void OSXRenderTarget::lockBackBuffer() {
        [NSGraphicsContext saveGraphicsState];
        [NSGraphicsContext setCurrentContext:[NSGraphicsContext
                                graphicsContextWithBitmapImageRep:d_backRenderBuffer]];
    }

    void OSXRenderTarget::unlockBackBuffer() {
        [NSGraphicsContext restoreGraphicsState];
    }

    void OSXRenderTarget::swapBuffers() {
        d_frontRenderBuffer = [[NSImage alloc] initWithCGImage:[d_backRenderBuffer CGImage] size:CGSizeMake(d_width, d_height)];
        d_frontRenderBuffer.name = @"d_frontRenderBuffer";
    }

    void OSXRenderTarget::beginFrame() {}
    void OSXRenderTarget::endFrame() {}

    void OSXRenderTarget::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        [[NSColor   colorWithCalibratedRed: r / 255.0f
                    green:g / 255.0f
                    blue:b / 255.0f
                    alpha:a / 255.0f]
                    set];

        NSRectFill(NSMakeRect(0, 0, d_width * d_scalingFactor, d_height * d_scalingFactor));
    }

    void OSXRenderTarget::pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);
        _convertPositionToCartesianCoordinates(y, height);

        [[NSGraphicsContext currentContext] saveGraphicsState];

		// Create the path and add the shapes
		NSBezierPath* clipPath = [NSBezierPath bezierPath];
		[clipPath appendBezierPathWithRect:NSMakeRect(x, y, width, height)];
		[clipPath addClip];
    }
    
    void OSXRenderTarget::popClipLayer()
    {
        [[NSGraphicsContext currentContext] restoreGraphicsState];
    }

    void OSXRenderTarget::drawRectangle(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        uint32_t radius,
        bool filled,
        uint32_t stroke
    ) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);
        _convertPositionToCartesianCoordinates(y, height);

        [[NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
            green:(float)color.g / 255.0f 
            blue:(float)color.b / 255.0f 
            alpha:(float)color.a / 255.0f] 
            set];

		NSBezierPath* path = [NSBezierPath bezierPathWithRoundedRect:NSMakeRect(x, y, width, height)
                                  xRadius:radius
                                  yRadius:radius];
		
		if (filled) {
			[path fill];
		}
		else {
			[path setLineWidth:stroke];
			[path stroke];
		}
    }

    void OSXRenderTarget::drawText(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle,
        const std::string& alignment,
        const std::string& wrapMode
    ) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);
        fontSize *= d_scalingFactor;

        NSColor* textColor = [NSColor colorWithCalibratedRed:(float)color.r / 255.0f 
									green:(float)color.g / 255.0f 
									blue:(float)color.b / 255.0f 
									alpha:(float)color.a / 255.0f];

		NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
		if (wrapMode == "none")
			[paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
		else if (wrapMode == "letter")
			[paragraphStyle setLineBreakMode:NSLineBreakByCharWrapping];
		else if (wrapMode == "word")
			[paragraphStyle setLineBreakMode:NSLineBreakByWordWrapping];

		NSFont* nsFont = [NSFont fontWithName:[NSString stringWithUTF8String: font.c_str()] size:fontSize];
        if (fontStyle == "bold") {
            nsFont = [[NSFontManager sharedFontManager] convertFont:nsFont toHaveTrait:NSFontBoldTrait];
        } else if (fontStyle == "italic") {
            nsFont = [[NSFontManager sharedFontManager] convertFont:nsFont toHaveTrait:NSFontItalicTrait];
        }

		NSDictionary* attribs = @{ 
									NSForegroundColorAttributeName : textColor,
									NSFontAttributeName : nsFont,
									NSParagraphStyleAttributeName: paragraphStyle
								};


        // Conversion provided text to widestring to support emojis and UTF-8
        auto wideText = utils::convertToWideString(text);

		NSString* str = [[NSString alloc] initWithBytes:wideText.c_str()
                                          length:wideText.size() * sizeof(wchar_t)
                                          encoding:NSUTF32LittleEndianStringEncoding];
                                          
		CGRect frame = [str boundingRectWithSize:NSMakeSize(width, height)
                                          	options:NSStringDrawingUsesLineFragmentOrigin
                                       		attributes:attribs
                                          	context:nil];

        CGSize stringSize = CGSizeMake(frame.size.width, frame.size.height + 1);
		NSRect layerRect = NSMakeRect(x, y, width, height);

		float xPos = 0;
		if (alignment == "center")
			xPos = layerRect.origin.x + (layerRect.size.width - stringSize.width) / 2; 
		else if (alignment == "left")
			xPos = layerRect.origin.x; 
		else if (alignment == "right")
			xPos = layerRect.origin.x + layerRect.size.width - stringSize.width;

		float yPos = layerRect.origin.y + layerRect.size.height / 2.0f - stringSize.height / 2.0f;

        int32_t yPosInt = static_cast<int32_t>(yPos);
        _convertPositionToCartesianCoordinates(yPosInt, height);
        yPos = static_cast<float>(yPosInt);

		[str drawInRect:NSMakeRect(xPos, yPos, width, height) withAttributes:attribs];
    }

    std::pair<float, float> OSXRenderTarget::runtimeCalculateTextSize(
        uint64_t maxWidth,
        uint64_t maxHeight,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle,
        const std::string& alignment,
        const std::string& wrapMode
    ) {
        maxWidth = static_cast<uint64_t>(static_cast<float>(maxWidth) * d_scalingFactor);
        maxHeight = static_cast<uint64_t>(static_cast<float>(maxHeight) * d_scalingFactor);

		NSMutableParagraphStyle* paragraphStyle = [[NSMutableParagraphStyle alloc] init];
		if (wrapMode == "none")
			[paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
		else if (wrapMode == "letter")
			[paragraphStyle setLineBreakMode:NSLineBreakByCharWrapping];
		else if (wrapMode == "word")
			[paragraphStyle setLineBreakMode:NSLineBreakByWordWrapping];

		NSFont* nsFont = [NSFont fontWithName:[NSString stringWithUTF8String: font.c_str()] size:fontSize];
        if (fontStyle == "bold") {
            nsFont = [[NSFontManager sharedFontManager] convertFont:nsFont toHaveTrait:NSFontBoldTrait];
        } else if (fontStyle == "italic") {
            nsFont = [[NSFontManager sharedFontManager] convertFont:nsFont toHaveTrait:NSFontItalicTrait];
        }

		NSDictionary* attribs = @{
									NSFontAttributeName : nsFont,
									NSParagraphStyleAttributeName: paragraphStyle
								};

		NSString* str = [NSString stringWithUTF8String: text.c_str()];
		CGRect frame = [str boundingRectWithSize:NSMakeSize(maxWidth, maxHeight)
                                          	options:NSStringDrawingUsesLineFragmentOrigin
                                       		attributes:attribs
                                          	context:nil];

        CGSize stringSize = CGSizeMake(frame.size.width, frame.size.height + 1);

		return {
            stringSize.width,
            stringSize.height
        };
    }
}