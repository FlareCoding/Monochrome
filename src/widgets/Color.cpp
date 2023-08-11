#include "Color.h"
#include <random>
#include <map>

namespace mc {
    std::map<std::string, Color> s_customColorMap;

    Color Color::randomColor() {
        static std::random_device rd;
        static std::mt19937 colorEngine(rd());

        std::uniform_int_distribution<std::uint32_t> distribution(0, 255);
        uint32_t r = distribution(colorEngine);
        uint32_t g = distribution(colorEngine);
        uint32_t b = distribution(colorEngine);

        return Color(r, g, b);
    }

    void Color::dim(uint8_t factor) {
        constexpr int baseAmount = 8;

        if (r > (baseAmount * (factor + 1))) r -= (baseAmount * factor);
        if (g > (baseAmount * (factor + 1))) g -= (baseAmount * factor);
        if (b > (baseAmount * (factor + 1))) b -= (baseAmount * factor);
    }

    void Color::undim(uint8_t factor) {
        constexpr int baseAmount = 8;

        if (r >= (baseAmount * factor) && r + (baseAmount * factor) <= 255)
            r += (baseAmount * factor);

        if (g >= (baseAmount * factor) && g + (baseAmount * factor) <= 255)
            g += (baseAmount * factor);

        if (b >= (baseAmount * factor) && b + (baseAmount * factor) <= 255)
            b += (baseAmount * factor);
    }

    Color Color::transparent    = Color(0, 0, 0, 0);
    Color Color::white          = Color(255, 255, 255);
    Color Color::black          = Color(0, 0, 0);
    Color Color::red            = Color(200, 0, 0);
    Color Color::green          = Color(0, 160, 0);
    Color Color::blue           = Color(0, 0, 160);
    Color Color::yellow         = Color(160, 160, 0);
    Color Color::gray           = Color(100, 100, 100);
    Color Color::lightGray      = Color(160, 160, 160);
    Color Color::darkGray       = Color(60, 60, 60);
    Color Color::orange         = Color(255, 140, 0);
    Color Color::cyan           = Color(0, 180, 180);
    Color Color::purple         = Color(128, 0, 128);
    Color Color::magenta        = Color(255, 0, 255);
    Color Color::pink           = Color(255, 192, 203);
    Color Color::salmon         = Color(250, 128, 114);
    Color Color::brown          = Color(139, 69, 19);
    Color Color::teal           = Color(0, 128, 128);
    Color Color::maroon         = Color(128, 0, 0);
    Color Color::lime           = Color(0, 255, 0);
    Color Color::violet         = Color(138, 43, 226);
    Color Color::olive          = Color(128, 128, 0);
    Color Color::coral          = Color(255, 127, 80);
    Color Color::chocolate      = Color(210, 105, 30);
    Color Color::indigo         = Color(75, 0, 130);
    Color Color::navy           = Color(0, 0, 128);

    void registerCustomColor(const std::string& name, const Color& color) {
        CORE_ASSERT(s_customColorMap.find(name) == s_customColorMap.end(),
            "Custom color with name '" + name + "' already exists");

        s_customColorMap.insert({ name, color });
    }

    Color getCustomColor(const std::string& name) {
        if (s_customColorMap.find(name) != s_customColorMap.end()) {
            return s_customColorMap.at(name);
        }

        printf("Failed to retrieve custom color: %s\n", name.c_str());
        return Color::transparent;
    }

namespace mcx {
    void registerMcxPrimitiveColorNames() {
        registerCustomColor("transparent", Color::transparent);
        registerCustomColor("white", Color::white);
        registerCustomColor("black", Color::black);
        registerCustomColor("red", Color::red);
        registerCustomColor("green", Color::green);
        registerCustomColor("blue", Color::blue);
        registerCustomColor("yellow", Color::yellow);
        registerCustomColor("gray", Color::gray);
        registerCustomColor("lightGray", Color::lightGray);
        registerCustomColor("darkGray", Color::darkGray);
        registerCustomColor("orange", Color::orange);
        registerCustomColor("cyan", Color::cyan);
        registerCustomColor("purple", Color::purple);
        registerCustomColor("magenta", Color::magenta);
        registerCustomColor("pink", Color::pink);
        registerCustomColor("salmon", Color::salmon);
        registerCustomColor("brown", Color::brown);
        registerCustomColor("teal", Color::teal);
        registerCustomColor("maroon", Color::maroon);
        registerCustomColor("lime", Color::lime);
        registerCustomColor("violet", Color::violet);
        registerCustomColor("olive", Color::olive);
        registerCustomColor("coral", Color::coral);
        registerCustomColor("chocolate", Color::chocolate);
        registerCustomColor("indigo", Color::indigo);
        registerCustomColor("navy", Color::navy);
    }
} // namespace mcx
} // namespace mc
