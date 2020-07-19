#pragma once
#include <string>

namespace mc
{
	enum class TextAlignment
	{
		LEADING,
		CENTERED,
		TRAILING
	};

	enum class TextStyle
	{
		DEFAULT,
		Light,
		ITALIC,
		BOLD,
		BOLD_ITALIC,
		SEMIBOLD,
		SEMIBOLD_ITALIC
	};

	struct TextProperties
	{
		std::string		Font = "Arial";
		unsigned int	FontSize = 12;

		TextAlignment	Allignment = TextAlignment::CENTERED;
		TextStyle		Style = TextStyle::DEFAULT;
	};
}
