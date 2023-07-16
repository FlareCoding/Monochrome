#pragma once
#include <widgets/Color.h>
#include "Bitmap.h"

namespace mc {
class RenderTarget {
public:
    // Sizing functions
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;

    // Buffer management functions
    virtual void lockBackBuffer() = 0;
    virtual void unlockBackBuffer() = 0;
    virtual void swapBuffers() = 0;
    virtual void* getFrontBuffer() = 0;

    // Drawing functions
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual void beginOffscreenSceneFrame() = 0;
    virtual void endOffscreenSceneFrame() = 0;

    virtual void drawOffscreenSceneBitmap() = 0;

    virtual void clearScreen(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;

    virtual void pushClipLayer(int32_t x, int32_t y, uint32_t width, uint32_t height) = 0;
    virtual void popClipLayer() = 0;

    virtual void drawRectangle(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        uint32_t radius,
        bool filled,
        uint32_t stroke
    ) = 0;

    virtual void drawCircle(
        int32_t x, int32_t y,
        uint32_t radius,
        const Color& color,
        bool filled,
        uint32_t stroke
    ) = 0;

    virtual void drawText(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        const std::string& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle = "normal",
        const std::string& alignment = "center",
        const std::string& wrapMode = "none"
    ) = 0;

    virtual void drawText(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        const Color& color,
        const std::wstring& text,
        const std::string& font,
        uint32_t fontSize,
        const std::string& fontStyle = "normal",
        const std::string& alignment = "center",
        const std::string& wrapMode = "none"
    ) = 0;

    virtual void drawBitmap(
        int32_t x, int32_t y,
        uint32_t width, uint32_t height,
        Shared<Bitmap> bitmap,
        uint32_t opacity = 255
    ) = 0;
};
} // namespace mc
