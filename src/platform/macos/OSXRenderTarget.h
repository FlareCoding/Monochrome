#pragma once
#include <rendering/RenderTarget.h>
#include <Cocoa/Cocoa.h>

namespace mc {
class OSXRenderTarget : public RenderTarget {
public:
    OSXRenderTarget(NSView* contentView, float dpiScalingFactor);

    uint32_t getWidth() const override { return d_width; }
    uint32_t getHeight() const override { return d_height; }
    void resize(uint32_t width, uint32_t height) override;

    void lockBackBuffer() override;
    void unlockBackBuffer() override;
    void swapBuffers() override;
    void* getFrontBuffer() override { return d_frontRenderBuffer; }

    void beginFrame() override;
    void endFrame() override;

    void beginOffscreenSceneFrame() override;
    void endOffscreenSceneFrame() override;

    void drawOffscreenSceneBitmap() override;

    void clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) override;

    void pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) override;
    void popClipLayer() override;

    void drawRectangle(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        uint32_t radius,
        bool filled,
        uint32_t stroke
    ) override;

    void drawCircle(
        int32_t x, int32_t y,
        uint32_t radius,
        const Color& color,
        bool filled,
        uint32_t stroke
    ) override;

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
    ) override;

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
    ) override;

    void drawBitmap(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        Shared<Bitmap> bitmap,
        uint32_t opacity = 255
    ) override;

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
    uint32_t    d_width;
    uint32_t    d_height;
    float       d_scalingFactor;

    NSView* d_contentView;
    NSBitmapImageRep* d_backRenderBuffer = nullptr;
    NSImage* d_frontRenderBuffer = nullptr;

    NSBitmapImageRep* d_offscreenSceneBitmap = nullptr;
    NSImage* d_offscreenSceneImage = nullptr;

    void _adjustPositionAndSizeForDPIScaling(
        int32_t& xPos,
        int32_t& yPos,
        uint32_t& width,
        uint32_t& height
    );

    void _convertPositionToCartesianCoordinates(int32_t& yPos, uint32_t height);
};
} // namespace mc
