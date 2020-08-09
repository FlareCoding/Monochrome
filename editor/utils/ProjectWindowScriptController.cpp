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
}

void ProjectWindowScriptController::DragView()
{
	while (!m_MouseButtonReleasedFromView)
	{
		if (m_TargetView->srcwindow)
			m_TargetView->layer.frame.position = m_TargetView->srcwindow->GetMouseCursorPos() - m_ViewClickedLocationOffset;

		Sleep(2);
	}
}
