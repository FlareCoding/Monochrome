#pragma once
#include <Monochrome.h>
using namespace mc;

class ProjectWindowScriptController
{
public:
	void Widget_OnMousePressed(Ref<UIView> view);
	void Widget_OnMouseReleased();

private:
	Ref<UIView> m_TargetView = nullptr;
	Size m_ViewClickedLocationOffset = { 0, 0 };
	bool m_MouseButtonReleasedFromView = true;

private:
	void DragView();
};
