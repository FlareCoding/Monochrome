#pragma once
#include <core/Core.h>

namespace mc
{
    struct Position
    {
        int32_t x;
        int32_t y;

        Position() = default;
        Position(int32_t x, int32_t y) : x(x), y(y) {}
    };

    using Point = Position;

    struct Size
    {
        uint32_t width;
        uint32_t height;

        Size() = default;
        Size(uint32_t width, uint32_t height) : width(width), height(height) {}
    };

    struct Distance
    {
        int32_t x;
        int32_t y;

        Distance() = default;
        Distance(int32_t x, int32_t y) : x(x), y(y) {}
    };

    struct Frame
    {
        Position position;
        Size size;

        Frame(
            const Position& position = { 0, 0 },
            const Size& size = { 0, 0 }
        ) : position(position), size(size) {}

        inline bool containsPoint(Position point) const {
            return (
                point.x > position.x && point.x < position.x + (int32_t)size.width &&
				point.y > position.y && point.y < position.y + (int32_t)size.height
            );
        }
    };
}
