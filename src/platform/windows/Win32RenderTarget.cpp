#include "Win32RenderTarget.h"
#include <application/AppManager.h>
#include <utils/TextUtils.h>

namespace mc
{
    Win32RenderTarget::Win32RenderTarget(HWND windowHandle, float dpiScalingFactor) {
        d_width = 0;
        d_height = 0;
        d_windowHandle = windowHandle;
        d_scalingFactor = dpiScalingFactor;

        // Initialize the graphics resources if needed
        if (!D2DGraphics::isInitialized()) {
            D2DGraphics::initialize();
        }

        // Initialize render target
        d_nativeWindowRenderTarget = D2DGraphics::createWindowRenderTarget(d_windowHandle);
        d_nativeBitmapRenderTarget = D2DGraphics::createBitmapRenderTarget(d_nativeWindowRenderTarget.Get());

        // Initialize the front buffer
        d_frontRenderBuffer = D2DGraphics::createBitmap(d_nativeWindowRenderTarget, d_width, d_height);

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

    void Win32RenderTarget::resize(uint32_t width, uint32_t height) {
        d_width = width;
        d_height = height;

        RECT rect;
        GetClientRect(d_windowHandle, &rect);

        UINT adjustedWidth = (UINT)((float)(rect.right - rect.left) * d_scalingFactor);
        UINT adjustedHeight = (UINT)((float)(rect.bottom - rect.top) * d_scalingFactor);

        // Update the front buffer render target
        d_nativeWindowRenderTarget->Resize(D2D1::SizeU(adjustedWidth, adjustedHeight));

        // Re-create the back buffer render target
        d_nativeBitmapRenderTarget = D2DGraphics::createBitmapRenderTarget(d_nativeWindowRenderTarget.Get());

        // Re-create the front  buffer bitmap
        d_frontRenderBuffer = D2DGraphics::createBitmap(d_nativeWindowRenderTarget, d_width, d_height);
    }

    void Win32RenderTarget::_adjustPositionAndSizeForDPIScaling(
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

    void Win32RenderTarget::lockBackBuffer() {
        d_activeRenderTarget = d_nativeBitmapRenderTarget;
    }

    void Win32RenderTarget::unlockBackBuffer() {
        d_activeRenderTarget = d_nativeWindowRenderTarget;
    }

    void Win32RenderTarget::swapBuffers() {
        d_frontRenderBuffer->CopyFromRenderTarget(NULL, d_nativeBitmapRenderTarget.Get(), NULL);
    }

    void Win32RenderTarget::beginFrame() {
        d_activeRenderTarget->BeginDraw();
    }

    void Win32RenderTarget::endFrame() {
        d_activeRenderTarget->EndDraw();
    }

    void Win32RenderTarget::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        d_activeRenderTarget->Clear(D2D1::ColorF(
            (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f)
        );
    }

    void Win32RenderTarget::pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);

        D2D1_RECT_F bounds = D2D1::RectF(
            (float)x,
            (float)y,
            (float)(x + width),
            (float)(y + height)
        );
        d_activeRenderTarget->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }

    void Win32RenderTarget::popClipLayer() {
        d_activeRenderTarget->PopAxisAlignedClip();
    }

    void Win32RenderTarget::drawRectangle(
        int32_t x,
        int32_t y,
        uint32_t width,
        uint32_t height,
        const Color& color,
        uint32_t radius,
        bool filled,
        uint32_t stroke
    ) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);

        ID2D1SolidColorBrush* brush;
        HRESULT result = d_activeRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(
                (float)color.r / 255.0f,
                (float)color.g / 255.0f,
                (float)color.b / 255.0f,
                (float)color.a / 255.0f
            ),
            &brush
        );

        CORE_ASSERT(result == S_OK, "Failed to create brush");

        D2D1_RECT_F bounds = D2D1::RectF(
            (float)x,
            (float)y,
            (float)(x + width),
            (float)(y + height)
        );

        if (radius) {
            D2D1_ROUNDED_RECT roundedBounds = D2D1::RoundedRect(bounds, (float)radius, (float)radius);

            if (filled)
                d_activeRenderTarget->FillRoundedRectangle(roundedBounds, brush);
            else
                d_activeRenderTarget->DrawRoundedRectangle(roundedBounds, brush, (float)stroke);
        }
        else {
            if (filled)
                d_activeRenderTarget->FillRectangle(bounds, brush);
            else
                d_activeRenderTarget->DrawRectangle(bounds, brush, (float)stroke);
        }

        brush->Release();
    }

    void Win32RenderTarget::drawText(
        int32_t x,
        int32_t y,
        uint32_t width,
        uint32_t height,
        const Color& color,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle,
        const std::string& alignment,
        const std::string& wrapMode
    ) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);

        // Adjust the font size for DPI scaling
        float fontSizeF = static_cast<float>(fontSize);
        fontSize = static_cast<uint32_t>(fontSizeF * d_scalingFactor);

        // Convert text to be a wide string
        std::wstring textW = utils::convertToWideString(text);

        // Create the brush
        ID2D1SolidColorBrush* brush;
        HRESULT result = d_activeRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(
                (float)color.r / 255.0f,
                (float)color.g / 255.0f,
                (float)color.b / 255.0f,
                (float)color.a / 255.0f
            ),
            &brush
        );

        CORE_ASSERT(result == S_OK, "Failed to create brush");

        // Calculate the font weight and style
        auto dwFontWeight = DWRITE_FONT_WEIGHT_NORMAL;
        auto dwFontStyle = DWRITE_FONT_STYLE_NORMAL;

        if (fontStyle == "bold")
            dwFontWeight = DWRITE_FONT_WEIGHT_BOLD;
        else if (fontStyle == "italic")
            dwFontStyle = DWRITE_FONT_STYLE_ITALIC;

        // Create the text format
        IDWriteTextFormat* format;
        result = D2DGraphics::getDWriteFactory()->CreateTextFormat(
            std::wstring(font.begin(), font.end()).c_str(),
            NULL,
            dwFontWeight,
            dwFontStyle,
            DWRITE_FONT_STRETCH_NORMAL,
            (float)fontSize,
            L"",
            &format
        );
        CORE_ASSERT(result == S_OK, "Failed to create text format");

        // Calculate text wrapping mode
        auto dwWrapMode = DWRITE_WORD_WRAPPING_NO_WRAP;
        
        if (wrapMode == "letter")
            dwWrapMode = DWRITE_WORD_WRAPPING_CHARACTER;
        else if (wrapMode == "word")
            dwWrapMode = DWRITE_WORD_WRAPPING_WHOLE_WORD;
        
        // Calculate text alignment
        auto dwTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;

        if (alignment == "left")
            dwTextAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
        else if (alignment == "right")
            dwTextAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;

        format->SetTextAlignment(dwTextAlignment);
        format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        format->SetWordWrapping(dwWrapMode);

        d_activeRenderTarget->DrawTextW(
            textW.c_str(),
            (UINT32)text.size(),
            format,
            D2D1::RectF((float)x,
                (float)y,
                (float)(x + width),
                (float)(y + height)
            ),
            brush,
            D2D1_DRAW_TEXT_OPTIONS_CLIP
        );
        
        brush->Release();
        format->Release();
    }
    
    void Win32RenderTarget::__drawFrontBufferBitmap() {
        d_nativeWindowRenderTarget->BeginDraw();

        D2D1_RECT_F srcRect = D2D1::RectF(
            0.0f,
            0.0f,
            d_frontRenderBuffer->GetSize().width,
            d_frontRenderBuffer->GetSize().height
        );

        D2D1_RECT_F destRect = D2D1::RectF(
            0.0f,
            0.0f,
            (float)d_width,
            (float)d_height
        );

        d_nativeWindowRenderTarget->DrawBitmap(
            d_frontRenderBuffer.Get(),
            srcRect,
            1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            destRect
        );

        d_nativeWindowRenderTarget->EndDraw();
    }

    std::pair<float, float> Win32RenderTarget::runtimeCalculateTextSize(
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

        // Adjust the font size for DPI scaling
        float fontSizeF = static_cast<float>(fontSize);
        fontSize = static_cast<uint32_t>(fontSizeF * d_scalingFactor);

        // Convert text to be a wide string
        std::wstring textW = utils::convertToWideString(text);

        // Calculate the font weight and style
        auto dwFontWeight = DWRITE_FONT_WEIGHT_NORMAL;
        auto dwFontStyle = DWRITE_FONT_STYLE_NORMAL;

        if (fontStyle == "bold")
            dwFontWeight = DWRITE_FONT_WEIGHT_BOLD;
        else if (fontStyle == "italic")
            dwFontStyle = DWRITE_FONT_STYLE_ITALIC;

        // Create the text format
        IDWriteTextFormat* format;
        HRESULT result = D2DGraphics::getDWriteFactory()->CreateTextFormat(
            std::wstring(font.begin(), font.end()).c_str(),
            NULL,
            dwFontWeight,
            dwFontStyle,
            DWRITE_FONT_STRETCH_NORMAL,
            (float)fontSize,
            L"",
            &format
        );
        CORE_ASSERT(result == S_OK, "Failed to create text format");

        // Calculate text wrapping mode
        auto dwWrapMode = DWRITE_WORD_WRAPPING_NO_WRAP;

        if (wrapMode == "letter")
            dwWrapMode = DWRITE_WORD_WRAPPING_CHARACTER;
        else if (wrapMode == "word")
            dwWrapMode = DWRITE_WORD_WRAPPING_WHOLE_WORD;

        // Calculate text alignment
        auto dwTextAlignment = DWRITE_TEXT_ALIGNMENT_CENTER;

        if (alignment == "left")
            dwTextAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
        else if (alignment == "right")
            dwTextAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;

        format->SetTextAlignment(dwTextAlignment);
        format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        format->SetWordWrapping(dwWrapMode);

        // Creating text layout to get mertrics from
        IDWriteTextLayout* layout;
        result = D2DGraphics::getDWriteFactory()->CreateTextLayout(
            std::wstring(text.begin(), text.end()).c_str(),
            (UINT32)text.size(),
            format,
            (float)maxWidth,
            (float)maxHeight,
            &layout
        );
        CORE_ASSERT(result == S_OK, "Failed to create text layout");

        DWRITE_TEXT_METRICS dwriteMetrics;
        layout->GetMetrics(&dwriteMetrics);

        format->Release();
        layout->Release();
        return {
            dwriteMetrics.widthIncludingTrailingWhitespace,
            dwriteMetrics.height
        };
    }
}