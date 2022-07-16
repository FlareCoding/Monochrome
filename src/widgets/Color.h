#pragma once
#include <core/Core.h>

namespace mc {
class Color {
public:
    uint8_t r, g, b, a;

    Color() :  r(0), g(0), b(0), a(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}

    bool operator==(const Color& color) const {
        return (r == color.r && g == color.g && b == color.b && a == color.a);
    }

    bool operator!=(const Color& color) const {
        return (r != color.r || g != color.g || b != color.b || a != color.a);
    }

    static Color transparent;
    static Color white;
    static Color black;
    static Color red;
    static Color green;
    static Color blue;
    static Color yellow;
    static Color gray;
    static Color lightGray;
    static Color darkGray;
    static Color teal;
    static Color magenta;
};
} // namespace mc
