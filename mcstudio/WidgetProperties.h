#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>

#include "StudioUtils.h"

namespace WidgetProperties
{
	namespace Flags
	{
		constexpr unsigned char TOGGLABLE	= 0x00000001;
		constexpr unsigned char DATA_ENTRY	= 0x00000002;
		constexpr unsigned char SELECTABLE	= 0x00000004;
	}

	using ApplierFn_t = std::function<void(UIView* widget, const std::string& value)>;
	using GetterFn_t = std::function<std::string(UIView* widget)>;
	struct Property
	{
		unsigned char type = 0;
		std::string name;
		std::vector<std::string> items; // used by selectable properties

		ApplierFn_t applier_fn;
		GetterFn_t getter_fn;
	};

	using Properties_t = std::vector<Property>;
	using PropertiesList_t = std::map<WidgetType, Properties_t>;

	extern PropertiesList_t WidgetPropertiesList;

	void CreatePropertiesList();
}
