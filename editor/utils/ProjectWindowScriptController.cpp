#include "ProjectWindowScriptController.h"

void ProjectWindowScriptController::Widget_OnMousePressed(Ref<UIView> view)
{
	m_MouseButtonReleasedFromView = false;
	m_TargetView = view;
	m_ViewClickedLocationOffset = view->srcwindow->GetMouseCursorPos() - view->GetAbsolutePosition();

	std::thread dragging_thread(&ProjectWindowScriptController::DragView, this);
	dragging_thread.detach();
}

void ProjectWindowScriptController::Widget_OnMouseReleased()
{
	m_MouseButtonReleasedFromView = true;

	if (m_TargetView)
	{
		CheckEmbeddingStatus();
	}
}

void ProjectWindowScriptController::FindInnerMostView(Ref<UIView> view, Ref<UIView>& target)
{
	if (!view) return;

	for (auto& child : view->subviews)
	{
		if (!IsViewEmbeddable(view.get())) continue;

		Frame frame = child->layer.frame;
		frame.position = child->GetAbsolutePosition();

		if (frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
		{
			target = child;
			FindInnerMostView(child, target);
		}
	}
}

void ProjectWindowScriptController::DragView()
{
	Position PreviousMousePosition = 
		m_TargetView->srcwindow ? m_TargetView->srcwindow->GetMouseCursorPos() : Position { 0, 0 };

	while (!m_MouseButtonReleasedFromView)
	{
		if (m_TargetView->srcwindow)
		{
			auto cursor_pos = m_TargetView->srcwindow->GetMouseCursorPos();
			auto pos = cursor_pos - m_ViewClickedLocationOffset;

			Size MouseMovedDistance = cursor_pos - PreviousMousePosition;
			PreviousMousePosition = cursor_pos;

			const auto CheckForResizingConditions = [this, MouseMovedDistance]() -> bool {
				static float VerticalBorderWidth = 10.0f;
				static float HorizontalBorderWidth = 20.0f;

				Position pt = m_ViewClickedLocationOffset;

				/*top-left, top and top-right*/
				if (pt.y < VerticalBorderWidth)
				{
					if (pt.x < HorizontalBorderWidth)
					{
						// Top Left
						m_TargetView->layer.frame.position += MouseMovedDistance;
						m_TargetView->layer.frame.size -= MouseMovedDistance;
						return true;
					}
					else if (pt.x > (m_TargetView->layer.frame.size.width - HorizontalBorderWidth))
					{
						// Top Right
						m_TargetView->layer.frame.size += MouseMovedDistance;
						return true;
					}
					
					// Top
					m_TargetView->layer.frame.position.y += MouseMovedDistance.y;
					m_TargetView->layer.frame.size.y -= MouseMovedDistance.y;
					return true;
				}
				/*bottom-left, bottom and bottom-right */
				if (pt.y > (m_TargetView->layer.frame.size.height - VerticalBorderWidth))
				{
					if (pt.x < HorizontalBorderWidth)
					{
						// Bottom Left
						m_TargetView->layer.frame.position.x += MouseMovedDistance.x;
						m_TargetView->layer.frame.size.x += MouseMovedDistance.x * 2.0f;
						m_TargetView->layer.frame.size.y += MouseMovedDistance.y * 2.0f;
						return true;
					}
					else if (pt.x > (m_TargetView->layer.frame.size.width - HorizontalBorderWidth))
					{
						// Bottom Right
						m_TargetView->layer.frame.size += MouseMovedDistance;
						return true;
					}

					// Bottom
					m_TargetView->layer.frame.size.y += MouseMovedDistance.y;
					return true;
				}
				if (pt.x < HorizontalBorderWidth)
				{
					// Left
					m_TargetView->layer.frame.position.x += MouseMovedDistance.x;
					m_TargetView->layer.frame.size.x -= MouseMovedDistance.x;
					return true;
				}
				if (pt.x > (m_TargetView->layer.frame.size.width - HorizontalBorderWidth))
				{
					// Right
					m_TargetView->layer.frame.size.x += MouseMovedDistance.x;
					return true;
				}

				return false;
			};

			bool resizing = CheckForResizingConditions();
			if (resizing)
				continue;

			if (utils::EditorSettings::GridSnapping)
			{
				auto grid_size = utils::EditorSettings::GridSize;
				Size offset = { (float)((uint32_t)pos.x % grid_size),  (float)((uint32_t)pos.y % grid_size) };

				if (offset.x > (grid_size / 2))
					pos.x += grid_size - offset.x;
				else
					pos.x -= offset.x;

				if (offset.y > (grid_size / 2))
					pos.y += grid_size - offset.y;
				else
					pos.y -= offset.y;
			}

			m_TargetView->layer.frame.position = pos;
			
			UIView* parent = m_TargetView->parent;
			while (parent)
			{
				m_TargetView->layer.frame.position -= parent->layer.frame.position;
				parent = parent->parent;
			}
		}

		Sleep(2);
	}
}

void ProjectWindowScriptController::CheckEmbeddingStatus()
{
	bool should_unembed = true;
	while (should_unembed)
	{
		should_unembed = UnembedViewIfNeeded(m_TargetView);
	}

	Ref<UIView> embeddable = nullptr;
	auto* elements = m_ProjectUIElements;
	while (CheckIfViewNeedsEmbedding(m_TargetView, *elements, embeddable))
		elements = &embeddable->subviews;

	if (embeddable && embeddable.get() != m_TargetView->parent)
	{
		if (m_TargetView->parent) m_TargetView->parent->RemoveSubview(m_TargetView);
		EmbedView(m_TargetView, embeddable);
	}
}

bool ProjectWindowScriptController::IsViewEmbeddable(UIView* view)
{
	if (utils::CheckType<UILabel>(view)) return false;
	if (utils::CheckType<UIButton>(view)) return false;
	if (utils::CheckType<UICheckbox>(view)) return false;
	if (utils::CheckType<UISlider>(view)) return false;
	if (utils::CheckType<UITextbox>(view)) return false;
	if (utils::CheckType<UICombobox>(view)) return false;

	return true;
}

bool ProjectWindowScriptController::CheckIfViewNeedsEmbedding(Ref<UIView>& view_being_checked, std::vector<Ref<UIView>>& elements, Ref<UIView>& embeddable)
{
	if (!view_being_checked->srcwindow)
		return false;

	for (auto& view : elements)
	{
		if (!view->srcwindow)
			continue;

		if (view.get() == view_being_checked.get()) continue;

		Frame frame = view->layer.frame;
		frame.position = view->GetAbsolutePosition();

		if (frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
		{
			embeddable = view;
			return true;
		}
	}

	return false;
}

void ProjectWindowScriptController::EmbedView(Ref<UIView> src, Ref<UIView> dest)
{
	auto it = std::find(m_ProjectUIElements->begin(), m_ProjectUIElements->end(), src);
	if (it != m_ProjectUIElements->end())
	{
		m_ProjectUIElements->erase(it);
		(*m_ProjectWindow)->RemoveView(src);
	}

	auto position_offset = dest->srcwindow->GetMouseCursorPos() - dest->GetAbsolutePosition();
	src->layer.frame.position = position_offset - m_ViewClickedLocationOffset;

	dest->AddSubview(src);
}

bool ProjectWindowScriptController::UnembedViewIfNeeded(Ref<UIView> view)
{
	bool result = false;

	// If the view inside another view, check if it needs to be taken out of it.
	if (view->parent)
	{
		Frame frame = view->parent->layer.frame;
		frame.position = view->parent->GetAbsolutePosition();

		if (!frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
		{
			UIView* parent_view = view->parent;
			view->layer.frame.position += parent_view->layer.frame.position;
			parent_view->RemoveSubview(m_TargetView);

			if (parent_view->parent)
			{
				parent_view->parent->AddSubview(view);
			}
			else
			{
				(*m_ProjectWindow)->AddView(view);
				m_ProjectUIElements->push_back(view);
			}

			result = true;
		}

		(*m_ProjectWindow)->ForceUpdate(true);
	}

	return result;
}
