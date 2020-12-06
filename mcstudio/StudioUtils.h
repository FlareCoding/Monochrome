#pragma once
#include <Monochrome.h>
using namespace mc;

enum class WidgetType
{
	Unknown,
    View,
	Label,
	Button,
	Checkbox,
	Slider,
    Textbox,
	TextArea,
    Image,
    ScrollPanel,
    DockingPanel,
    TabView
};

namespace StudioUtils
{
	template <typename T>
	bool CheckType(UIView* view)
	{
		return dynamic_cast<T*>(view);
	}

	bool IsViewEmbeddable(UIView* view);
	void FindInnerMostView(UIView* view, Ref<UIView>& target);
}
