#include "StudioUtils.h"

namespace StudioUtils
{
	bool IsViewEmbeddable(UIView* view)
	{
		if (CheckType<UILabel>(view)) return false;
		if (CheckType<UIButton>(view)) return false;
		if (CheckType<UICheckbox>(view)) return false;
		if (CheckType<UISlider>(view)) return false;
		if (CheckType<UITextbox>(view)) return false;
		if (CheckType<UICombobox>(view)) return false;

		return true;
	}

	void FindInnerMostView(UIView* view, Ref<UIView>& target)
	{
		if (!view) return;

		for (auto& child : view->subviews)
		{
			if (!IsViewEmbeddable(view)) continue;

			Frame frame = child->layer.frame;
			frame.position = child->GetAbsolutePosition();

			if (frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
			{
				target = child;
				FindInnerMostView(child.get(), target);
			}
		}
	}
}
