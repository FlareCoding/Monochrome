#pragma once
#include <string>
#include <cinttypes>

namespace mc
{
	class Color
	{
	public:
		Color() {}
		Color(uint32_t r, uint32_t g, uint32_t b, float alpha = 1.0f) : r(r), g(g), b(b), alpha(alpha) {}
		uint32_t r = 0, g = 0, b = 0;
		float alpha = 1.0f;

		/// @brief Converts a string into a color object.
		/// @param rgb String in a format "r g b (a)"
		/// @return Color instance.
		static Color FromRGBString(const char* rgb);

		std::string ToString();

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
