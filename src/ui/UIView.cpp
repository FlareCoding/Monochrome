#include "UIView.h"
#include <graphics/Graphics.h>
#include <window/UIWindow.h>
#include <algorithm>

namespace mc
{
	UIView::UIView(const Ref<UIView>& view)
	{
		layer = view->layer;
		subviews = view->subviews;
		parent = view->parent;
	}

	UIView::UIView(Frame frame)
	{
		layer.frame = frame;
	}

	void UIView::SetZIndex(uint32_t index)
	{
		m_zIndex = index;
		if (parent != nullptr)
			parent->SortElements();
	}

	Position UIView::GetAbsolutePosition()
	{
		Position pos = layer.frame.position;
		UIView* v = this;
		while (v->parent != nullptr)
		{
			pos += v->parent->layer.frame.position;
			v = v->parent;
		}
		return pos;
	}

	const uint32_t UIView::GetHighestLayerZIndex() const
	{
		if (!srcwindow && !parent)
			return GetZIndex();

		if (!parent)
			return srcwindow->GetHighestLayerZIndex();

		uint32_t idx = 0;
		for (auto& view : parent->subviews)
		{
			if (view->GetZIndex() > idx)
				idx = view->GetZIndex();
		}

		return idx;
	}

	void UIView::SortElements()
	{
		std::sort(subviews.begin(), subviews.end(), [](Ref<UIView>& lhs, Ref<UIView>& rhs) -> bool {
			return lhs->GetZIndex() < rhs->GetZIndex();
		});
	}

	void UIView::Draw()
	{
		Graphics::DrawRectangle(layer.frame.position.x, layer.frame.position.y, layer.frame.size.width, layer.frame.size.height, layer.color, CornerRadius, true);
	}
}
