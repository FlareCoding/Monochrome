#include "Win32RenderTarget.h"
#include <application/AppManager.h>
#include <utils/TextUtils.h>

namespace mc {
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
        d_nativeWindowRenderTarget =
            D2DGraphics::createWindowRenderTarget(d_windowHandle);

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
                ) {
                    return runtimeCalculateTextSize(
                        maxWidth,
                        maxHeight,
                        text,
                        font,
                        fontSize,
                        fontStyle,
                        alignment,
                        wrapMode
                    );
                };
        }
    }

    void Win32RenderTarget::resize(uint32_t width, uint32_t height) {
        d_width = width;
        d_height = height;

        RECT rect;
        GetClientRect(d_windowHandle, &rect);

        UINT adjustedWidth = (UINT)(static_cast<float>(rect.right - rect.left) * d_scalingFactor);
        UINT adjustedHeight = (UINT)(static_cast<float>(rect.bottom - rect.top) * d_scalingFactor);

        // Update the render target size
        d_nativeWindowRenderTarget->Resize(D2D1::SizeU(adjustedWidth, adjustedHeight));
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

    void Win32RenderTarget::lockBackBuffer() {}
    void Win32RenderTarget::unlockBackBuffer() {}
    void Win32RenderTarget::swapBuffers() {}

    void Win32RenderTarget::beginFrame() {
        d_nativeWindowRenderTarget->BeginDraw();
    }

    void Win32RenderTarget::endFrame() {
        d_nativeWindowRenderTarget->EndDraw();
    }

    void Win32RenderTarget::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        d_nativeWindowRenderTarget->Clear(
            D2D1::ColorF(
                static_cast<float>(r) / 255.0f,
                static_cast<float>(g) / 255.0f,
                static_cast<float>(b) / 255.0f
            )
        );
    }

    void Win32RenderTarget::pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);

        D2D1_RECT_F bounds = D2D1::RectF(
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(x + width),
            static_cast<float>(y + height)
        );
        d_nativeWindowRenderTarget->PushAxisAlignedClip(bounds, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    }

    void Win32RenderTarget::popClipLayer() {
        d_nativeWindowRenderTarget->PopAxisAlignedClip();
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
        HRESULT result = d_nativeWindowRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(color.a) / 255.0f
            ),
            &brush
        );

        CORE_ASSERT(result == S_OK, "Failed to create brush");

        D2D1_RECT_F bounds = D2D1::RectF(
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(x + width),
            static_cast<float>(y + height)
        );

        if (radius) {
            D2D1_ROUNDED_RECT roundedBounds =
                D2D1::RoundedRect(bounds, static_cast<float>(radius), static_cast<float>(radius));

            if (filled)
                d_nativeWindowRenderTarget->FillRoundedRectangle(roundedBounds, brush);
            else
                d_nativeWindowRenderTarget->DrawRoundedRectangle(roundedBounds, brush,
                    static_cast<float>(stroke));
        } else {
            if (filled)
                d_nativeWindowRenderTarget->FillRectangle(bounds, brush);
            else
                d_nativeWindowRenderTarget->DrawRectangle(
                    bounds, brush, static_cast<float>(stroke));
        }

        brush->Release();
    }

    void Win32RenderTarget::drawCircle(
        int32_t x, int32_t y,
        uint32_t radius,
        const Color& color,
        bool filled,
        uint32_t stroke
    ) {
        uint32_t size = radius * 2;
        _adjustPositionAndSizeForDPIScaling(x, y, size, size);

        ID2D1SolidColorBrush* brush;
        HRESULT result = d_nativeWindowRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(color.a) / 255.0f
            ),
            &brush
        );

        CORE_ASSERT(result == S_OK, "Failed to create brush");

        D2D1_ELLIPSE bounds = D2D1::Ellipse(
            D2D1::Point2F(
                static_cast<float>(x + size),
                static_cast<float>(y + size)
            ),
            static_cast<float>(size),
            static_cast<float>(size)
        );

        if (filled)
            d_nativeWindowRenderTarget->FillEllipse(bounds, brush);
        else
            d_nativeWindowRenderTarget->DrawEllipse(bounds, brush, static_cast<float>(stroke));

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
        drawText(
            x, y,
            width, height,
            color,
            utils::convertToWideString(text),
            font,
            fontSize,
            fontStyle,
            alignment,
            wrapMode
        );
    }

    void Win32RenderTarget::drawText(
        int32_t x,
        int32_t y,
        uint32_t width,
        uint32_t height,
        const Color& color,
        const std::wstring& text,
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

        // Create the brush
        ID2D1SolidColorBrush* brush;
        HRESULT result = d_nativeWindowRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(color.a) / 255.0f
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
            static_cast<float>(fontSize),
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

        d_nativeWindowRenderTarget->DrawTextW(
            text.c_str(),
            (UINT32)text.size(),
            format,
            D2D1::RectF(
                static_cast<float>(x),
                static_cast<float>(y),
                static_cast<float>(x + width),
                static_cast<float>(y + height)
            ),
            brush,
            D2D1_DRAW_TEXT_OPTIONS_CLIP
        );

        brush->Release();
        format->Release();
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
            static_cast<float>(fontSize),
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
            static_cast<float>(maxWidth),
            static_cast<float>(maxHeight),
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
} // namespace mc
