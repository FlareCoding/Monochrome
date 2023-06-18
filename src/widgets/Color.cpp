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
    Color Color::red            = Color(255, 0, 0);
    Color Color::green          = Color(0, 255, 0);
    Color Color::blue           = Color(0, 0, 255);
    Color Color::yellow         = Color(255, 255, 0);
    Color Color::gray           = Color(100, 100, 100);
    Color Color::lightGray      = Color(160, 160, 160);
    Color Color::darkGray       = Color(60, 60, 60);

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
} // namespace mc
