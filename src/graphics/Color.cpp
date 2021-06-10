#include "Color.h"
#include <vector>
#include <sstream>
#include <math.h>
#include <algorithm>

namespace mc
{
	std::vector<float> str2intarr(std::string str)
    {
		std::replace(str.begin(), str.end(), ',', ' ');

		std::vector<float> vect;
		std::stringstream ss(str);

		float n;
		while ((ss >> n))
			vect.push_back(n);

		return vect;
    }

	Color Color::transparent = Color(0, 0, 0, 0);
	Color Color::black = Color(0, 0, 0, 1);
	Color Color::white = Color(255, 255, 255, 1);
	Color Color::gray = Color(120, 120, 120, 1);
	Color Color::red = Color(255, 0, 0, 1);
	Color Color::green = Color(0, 255, 0, 1);
	Color Color::blue = Color(0, 0, 255, 1);
	Color Color::navy = Color(20, 26, 46, 1);
	Color Color::yellow = Color(255, 255, 0, 1);
	Color Color::orange = Color(255, 128, 0, 1);
	Color Color::purple = Color(127, 0, 255, 1);
	Color Color::cyan = Color(0, 255, 255, 1);
	Color Color::light_blue = Color(173, 216, 230, 1);

	Color Color::FromRGBString(const char* rgb)
	{
		auto values = str2intarr(rgb);
		Color instance = Color::black;
		
		if (values.size() >= 1)
			instance.r = (uint32_t)values[0];

		if (values.size() >= 2)
			instance.g = (uint32_t)values[1];

		if (values.size() >= 3)
			instance.b = (uint32_t)values[2];

		if (values.size() >= 4)
			instance.alpha = values[3];

		return instance;
	}

	std::string Color::ToString()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << alpha;

		return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + ss.str();
	}
}
