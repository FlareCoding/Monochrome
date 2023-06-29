#pragma once
#include <core/Core.h>

namespace mc {
struct Position {
    int32_t x;
    int32_t y;

    Position() = default;
    Position(int32_t x, int32_t y) : x(x), y(y) {}

    inline friend bool operator==(const Position& lhs, const Position& rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    inline friend Position operator+(const Position& lhs, const Position& rhs) {
        return {
            lhs.x + rhs.x,
            lhs.y + rhs.y
        };
    }

    inline friend Position operator-(const Position& lhs, const Position& rhs) {
        return {
            lhs.x - rhs.x,
            lhs.y - rhs.y
        };
    }

    inline Position& operator+=(const Position& rhs) {
        this->x += rhs.x;
        this->y += rhs.y;

        return *this;
    }

    inline Position& operator-=(const Position& rhs) {
        this->x -= rhs.x;
        this->y -= rhs.y;

        return *this;
    }
};

using Point = Position;

struct Size {
    uint32_t width;
    uint32_t height;

    Size() = default;
    Size(uint32_t width, uint32_t height) : width(width), height(height) {}

    inline friend bool operator==(const Size& lhs, const Size& rhs) {
        return (lhs.width == rhs.width) && (lhs.height == rhs.height);
    }
};

struct Distance {
    int32_t x;
    int32_t y;

    Distance() = default;
    Distance(int32_t x, int32_t y) : x(x), y(y) {}

    inline friend bool operator==(const Distance& lhs, const Distance& rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }
};

struct Frame {
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

    inline friend bool operator==(const Frame& lhs, const Frame& rhs) {
        return (lhs.position == rhs.position) && (lhs.size == rhs.size);
    }
};

enum class OverflowDirection {
    Down,
    Up,
    Left,
    Right
};

enum Orientation {
    Horizontal,
    Vertical
};

enum class DockAnchor {
    Top, Bottom,
    Left, Right,
    Fill
};
} // namespace mc
