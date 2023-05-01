#pragma once
#include <rendering/RenderTarget.h>

namespace mc {
class LinuxRenderTarget : public RenderTarget {
public:
    LinuxRenderTarget(void* windowHandle, float dpiScalingFactor);

    uint32_t getWidth() const { return d_width; }
    uint32_t getHeight() const { return d_height; }
    void resize(uint32_t width, uint32_t height);

    void lockBackBuffer();
    void unlockBackBuffer();
    void swapBuffers();
    void* getFrontBuffer() { return nullptr; }

    void beginFrame();
    void endFrame();
    void clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    void pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height);
    void popClipLayer();

    void drawRectangle(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        uint32_t radius,
        bool filled,
        uint32_t stroke
    );

    void drawCircle(
        int32_t x, int32_t y,
        uint32_t radius,
        const Color& color,
        bool filled,
        uint32_t stroke
    );

    void drawText(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle = "normal",
        const std::string& alignment = "center",
        const std::string& wrapMode = "none"
    );

    void drawText(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        const std::wstring& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle = "normal",
        const std::string& alignment = "center",
        const std::string& wrapMode = "none"
    );

    void drawBitmap(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        Shared<Bitmap> bitmap,
        uint32_t opacity = 255
    );

    std::pair<float, float> runtimeCalculateTextSize(
        uint64_t maxWidth,
        uint64_t maxHeight,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle,
        const std::string& alignment,
        const std::string& wrapMode
    );

private:
    void* d_windowHandle;

    uint32_t    d_width;
    uint32_t    d_height;
    float       d_scalingFactor;

    void _adjustPositionAndSizeForDPIScaling(
        int32_t& xPos,
        int32_t& yPos,
        uint32_t& width,
        uint32_t& height
    );
};
} // namespace mc