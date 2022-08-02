#include "Color.h"
#include <random>
namespace mc {
    
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
} // namespace mc
