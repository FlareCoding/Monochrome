#pragma once
#include <Monochrome.h>
using namespace mc;

enum class Widget
{
	Unknown,
	Label,
	Button,
	Checkbox,
	Slider
};

namespace utils
{
	std::string ColorToString(Color color);
	Color StringToColor(const std::string& str);
	int32_t StringToInt(const std::string& str);
	uint32_t StringToUInt(const std::string& str);
	std::string ConnectColorStrings(const std::string& r, const std::string& g, const std::string& b, const std::string& a);
	float StringToFloat(const std::string& str);
	double StringToDouble(const std::string& str);
	bool StringToBool(const std::string& str);
	TextAlignment StringToTextPropertiesAlignment(const std::string& str);
	TextStyle StringToTextPropertiesStyle(const std::string& str);
	WordWrapping StringToTextPropertiesWrapping(const std::string& str);

	template <typename T>
	bool CheckType(UIView* view)
	{
		return dynamic_cast<T*>(view);
	}

	class EditorSettings
	{
	public:
		static bool			GridSnapping;
		static uint32_t		GridSize;

		static bool			ProjectAutoSaving;
		static uint32_t		ProjectAutoSavingInterval;
	};
};
