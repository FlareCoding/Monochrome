#include "EditorUtils.h"

namespace utils
{
	std::string ColorToString(Color color)
	{
		std::string s;

		s.append(std::to_string(color.r));
		s.append(" ");
		s.append(std::to_string(color.g));
		s.append(" ");
		s.append(std::to_string(color.b));
		s.append(" ");
		s.append(std::to_string((uint32_t)color.alpha));

		return s;
	}

	Color StringToColor(const std::string& str)
	{
		Color color = Color::black;

		std::stringstream iss(str);
		std::vector<int> inputs;
		int n;

		while (iss >> n)
			inputs.push_back(n);

		if (inputs.size() < 3 || inputs.size() > 4) throw std::exception("incorrect number of color inputs");

		color.r = (uint32_t)inputs[0];
		color.g = (uint32_t)inputs[1];
		color.b = (uint32_t)inputs[2];

		if (inputs.size() == 4)
			color.alpha = (float)inputs[3];

		return color;
	}

	// ============================================================================== //
	// ============================================================================== //
	// ============================================================================== //

	bool		EditorSettings::GridSnapping	= false;
	uint32_t	EditorSettings::GridSize		= 10;

	bool		EditorSettings::ProjectAutoSaving			= false;
	uint32_t	EditorSettings::ProjectAutoSavingInterval	= 10;
}
