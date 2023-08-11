#pragma once
#include <core/Core.h>

namespace mc {
class Color {
public:
    uint8_t r, g, b, a;

    Color() :  r(0), g(0), b(0), a(255) {}
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}

    static Color randomColor();

    void dim(uint8_t factor = 1);
    void undim(uint8_t factor = 1);

    bool operator==(const Color& color) const {
        return (r == color.r && g == color.g && b == color.b && a == color.a);
    }

    bool operator!=(const Color& color) const {
        return (r != color.r || g != color.g || b != color.b || a != color.a);
    }

    inline bool isTransparent() const {
        return a == 0;
    }

    inline bool hasTransparency() const {
        return a < 255;
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
    static Color orange;
    static Color cyan;
    static Color purple;
    static Color magenta;
    static Color pink;
    static Color salmon;
    static Color brown;
    static Color teal;
    static Color maroon;
    static Color lime;
    static Color violet;
    static Color olive;
    static Color coral;
    static Color chocolate;
    static Color indigo;
    static Color navy;
};

// Allows to save specific color under a specified name
void registerCustomColor(const std::string& name, const Color& color);

// Allows to retrieve a saved specific color given its specified name
Color getCustomColor(const std::string& name);

namespace mcx {
    // Registers basic pre-defined color names
    void registerMcxPrimitiveColorNames();
}
} // namespace mc
