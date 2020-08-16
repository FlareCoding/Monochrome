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

	std::string ConnectColorStrings(const std::string& r, const std::string& g, const std::string& b, const std::string& a)
	{
		std::string s;
		s.append(r);
		s.append(" ");
		s.append(g);
		s.append(" ");
		s.append(b);
		s.append(" ");
		s.append(a);
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

	int32_t StringToInt(const std::string& str)
	{
		int32_t num = 0;
		int i = 0;
		bool isNegative = false;

		if (str[0] == '-')
		{
			isNegative = true;
			i = 1;
		}

		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		{
			num = num * 10 + (str[i] - '0');
			++i;
		}

		if (isNegative)
		{
			num = (-1) * num;
		}

		return num;
	}

	uint32_t StringToUInt(const std::string &str)
	{
		uint32_t num = 0;
		int i = 0;

		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		{
			num = num * 10 + (str[i] - '0');
			++i;
		}

		return num;
	}

	float StringToFloat(const std::string& str)
	{
		return (float)std::stod(str);
	}

	double StringToDouble(const std::string& str)
	{
		return std::stod(str);
	}

	bool StringToBool(const std::string& str)
	{
		return StringToInt(str);
	}

	TextAlignment StringToTextPropertiesAlignment(const std::string& str)
	{
		TextAlignment alignment = TextAlignment::CENTERED;

		if (str == "mc::TextAlignment::TRAILING")
			alignment = TextAlignment::TRAILING;
		else if (str == "mc::TextAlignment::LEADING")
			alignment = TextAlignment::LEADING;

		return alignment;
	}

	TextStyle StringToTextPropertiesStyle(const std::string& str)
	{
		TextStyle style = TextStyle::DEFAULT;
		
		if (str == "mc::TextStyle::BOLD")
			style = TextStyle::BOLD;
		else if (str == "mc::TextStyle::BOLD_ITALIC")
			style = TextStyle::BOLD_ITALIC;
		else if (str == "mc::TextStyle::ITALIC")
			style = TextStyle::ITALIC;
		else if (str == "mc::TextStyle::Light")
			style = TextStyle::Light;
		else if (str == "mc::TextStyle::SEMIBOLD")
			style = TextStyle::SEMIBOLD;
		else if (str == "mc::TextStyle::SEMIBOLD_ITALIC")
			style = TextStyle::SEMIBOLD_ITALIC;

		return style;
	}

	WordWrapping StringToTextPropertiesWrapping(const std::string& str)
	{
		WordWrapping wrapping = WordWrapping::NORMAL_WRAP;

		if (str == "mc::WordWrapping::CHARACTER_WRAP")
			wrapping = WordWrapping::CHARACTER_WRAP;
		else if (str == "mc::WordWrapping::EMERGENCY_BREAK")
			wrapping = WordWrapping::EMERGENCY_BREAK;
		else if (str == "mc::WordWrapping::NO_WRAP")
			wrapping = WordWrapping::NO_WRAP;
		else if (str == "mc::WordWrapping::WORD_WRAP")
			wrapping = WordWrapping::WORD_WRAP;

		return wrapping;
	}

	// ============================================================================== //
	// ============================================================================== //
	// ============================================================================== //

	bool		EditorSettings::GridSnapping				= false;
	uint32_t	EditorSettings::GridSize					= 10;

	bool		EditorSettings::ProjectAutoSaving			= false;
	uint32_t	EditorSettings::ProjectAutoSavingInterval	= 10;
}
