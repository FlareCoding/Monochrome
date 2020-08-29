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

	enum class WordWrapping
	{
		NORMAL_WRAP,
		NO_WRAP,
		CHARACTER_WRAP,
		WORD_WRAP,
		EMERGENCY_BREAK
	};

	struct TextProperties
	{
		std::string		Font = "Arial";
		unsigned int	FontSize = 12;

		TextAlignment	Alignment = TextAlignment::CENTERED;
		TextAlignment	VerticalAlignment = TextAlignment::CENTERED;
		TextStyle		Style = TextStyle::DEFAULT;
		WordWrapping	Wrapping = WordWrapping::NORMAL_WRAP;
	};

	struct TextMetrics
	{
		float Left;
		float Top;
		float Width;
		float WidthIncludingTrailingWhitespace;
		float Height;
		uint32_t LineCount;
	};
}
