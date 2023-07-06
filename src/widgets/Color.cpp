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

    Color Color::transparent    = Color(0, 0, 0, 0);
    Color Color::white          = Color(255, 255, 255);
    Color Color::black          = Color(0, 0, 0);
    Color Color::red            = Color(160, 0, 0);
    Color Color::green          = Color(0, 160, 0);
    Color Color::blue           = Color(0, 0, 160);
    Color Color::yellow         = Color(160, 160, 0);
    Color Color::gray           = Color(100, 100, 100);
    Color Color::lightGray      = Color(160, 160, 160);
    Color Color::darkGray       = Color(60, 60, 60);
    Color Color::orange         = Color();
    Color Color::cyan           = Color();
    Color Color::purple         = Color();
    Color Color::magenta        = Color();
    Color Color::pink           = Color();
    Color Color::salmon         = Color();
    Color Color::brown          = Color();
    Color Color::teal           = Color();
    Color Color::maroon         = Color();
    Color Color::lime           = Color();
    Color Color::violet         = Color();
    Color Color::olive          = Color();
    Color Color::coral          = Color();
    Color Color::chocolate      = Color();
    Color Color::indigo         = Color();
    Color Color::navy           = Color();

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
