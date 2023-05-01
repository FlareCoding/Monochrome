#include "LinuxRenderTarget.h"
#include <application/AppManager.h>
#include <utils/TextUtils.h>

namespace mc {
    LinuxRenderTarget::LinuxRenderTarget(void* windowHandle, float dpiScalingFactor) {
        d_width = 0;
        d_height = 0;
        d_windowHandle = windowHandle;
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
                ) {
                    return std::pair<float, float>(0, 0);
                };
        }
    }

    void LinuxRenderTarget::resize(uint32_t width, uint32_t height) {
        d_width = width;
        d_height = height;
    }

    void LinuxRenderTarget::_adjustPositionAndSizeForDPIScaling(
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

    void LinuxRenderTarget::lockBackBuffer() {}
    void LinuxRenderTarget::unlockBackBuffer() {}
    void LinuxRenderTarget::swapBuffers() {}

    void LinuxRenderTarget::beginFrame() {
    }

    void LinuxRenderTarget::endFrame() {
    }

    void LinuxRenderTarget::clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    }

    void LinuxRenderTarget::pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);
    }

    void LinuxRenderTarget::popClipLayer() {
    }

    void LinuxRenderTarget::drawRectangle(
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
    }

    void LinuxRenderTarget::drawCircle(
        int32_t x, int32_t y,
        uint32_t radius,
        const Color& color,
        bool filled,
        uint32_t stroke
    ) {
        uint32_t size = radius * 2;
        _adjustPositionAndSizeForDPIScaling(x, y, size, size);
    }

    void LinuxRenderTarget::drawText(
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

    void LinuxRenderTarget::drawText(
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
    }

    void LinuxRenderTarget::drawBitmap(
        int32_t x,
        int32_t y,
        uint32_t width,
        uint32_t height,
        Shared<Bitmap> bitmap,
        uint32_t opacity
    ) {
        _adjustPositionAndSizeForDPIScaling(x, y, width, height);
    }

    std::pair<float, float> LinuxRenderTarget::runtimeCalculateTextSize(
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

        return { maxWidth, maxHeight };
    }
} // namespace mc
