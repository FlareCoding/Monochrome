#pragma once
#include <widgets/Layer.h>
#include <widgets/Color.h>
#include <vector>
#include <mutex>

namespace mc {
class RenderTarget;

enum CanvasDrawOperation {
    DrawRectangle,
    DrawCircle,
    DrawText
};

class CanvasDrawCommand {
public:
    virtual const CanvasDrawOperation getOpType() = 0;
};

class CanvasDrawRectangleCommand : public CanvasDrawCommand {
public:
    const CanvasDrawOperation getOpType() override { return DrawRectangle; }

    Position    position;
    Size        size;
    Color       color;
    bool        filled;
    uint32_t    stroke;
    uint32_t    radius;
};

class CanvasDrawCircleCommand : public CanvasDrawCommand {
public:
    const CanvasDrawOperation getOpType() override { return DrawCircle; }

    Position    position;
    Color       color;
    bool        filled;
    uint32_t    stroke;
    uint32_t    radius;
};

class CanvasDrawTextCommand : public CanvasDrawCommand {
public:
    const CanvasDrawOperation getOpType() override { return DrawText; }

    Position    position;
    Size        size;
    Color       color;
    std::string text;
    std::string font;
    uint32_t    fontSize;
    std::string fontStyle;
    std::string alignment;
};

class Canvas2D {
public:
    Canvas2D() = default;

    inline void setRenderTarget(Shared<RenderTarget>& renderTarget) {
        d_renderTarget = renderTarget;
    }

    void __render();

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    Size getSize() const;

    void clear();

    void strokeRectangle(
        const Position& position,
        const Size& size,
        const Color& color = Color::white,
        uint32_t thickness = 2,
        uint32_t radius = 0
    );

    void fillRectangle(
        const Position& position,
        const Size& size,
        const Color& color = Color::white,
        uint32_t radius = 0
    );

    void strokeCircle(
        const Position& position,
        uint32_t radius,
        const Color& color = Color::white,
        uint32_t thickness = 2
    );

    void fillCircle(
        const Position& position,
        uint32_t radius,
        const Color& color = Color::white
    );

    void fillText(
        const Position& position,
        const Size& size,
        const Color& color,
        const std::string& text,
        const std::string& font = "Arial",
        uint32_t fontSize = 12,
        const std::string& fontStyle = "normal",
        const std::string& alignment = "center"
    );

private:
    // Reference to the current render target set by the window
    Shared<RenderTarget> d_renderTarget;

    // Buffer of commands to execute when rendering the scene
    std::vector<Shared<CanvasDrawCommand>> d_commandBuffer;

    // Mutex to protect clearing the buffer during the drawing phase
    std::mutex d_commandBufferMutex;
};
} // namespace mc
