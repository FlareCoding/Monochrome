#pragma once
#include <cinttypes>

namespace mc
{
	class Color
	{
	public:
		Color() {}
		Color(uint32_t r, uint32_t g, uint32_t b, float alpha = 1.0f) : r(r), g(g), b(b), alpha(alpha) {}
		Color(uint32_t r, uint32_t g, uint32_t b) : r(r), g(g), b(b), alpha(1.0f) {}
		uint32_t r = 0, g = 0, b = 0;
		float alpha = 1.0f;

		//static Color from_rgb_string(const char* rgb);
		//static Color from_hex_string(const char* hex);

		bool operator==(const Color& color) { return (r == color.r && g == color.g && b == color.b && alpha == color.alpha); }
		bool operator!=(const Color& color) { return (r != color.r || g != color.g || b != color.b || alpha != color.alpha); }

		static Color transparent;
		static Color black;
		static Color white;
		static Color gray;
		static Color red;
		static Color green;
		static Color blue;
		static Color navy;
		static Color yellow;
		static Color orange;
		static Color purple;
		static Color cyan;
		static Color light_blue;
	};
}
