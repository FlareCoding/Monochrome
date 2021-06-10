#pragma once
#include "ProjectView.h"

class MonochromeStudio
{
public:
	void Start();
	void Initialize();

private:
	void CreateStudioWindow();
	void InitStudioUI();

private:
	Ref<UIWindow> m_StudioWindow;

private:
	Ref<UIDockPanel> m_DockPanel;
	Ref<UITabView> m_WorkspaceTabView;

	Ref<UIView> m_ProjectSettingsBar;
	Ref<UITextbox> m_ProjectWidthTextbox;
	Ref<UITextbox> m_ProjectHeightTextbox;

	Ref<ProjectView> m_ProjectView;
	Ref<UILabel> m_ProjectViewResizeLabel;

	Ref<UIScrollPanel> m_Toolbox;
	Ref<UIScrollPanel> m_PropertiesPanel;

private:
	void DragResizeLabel();
	bool m_MouseButtonReleasedFromView = true;
	Point m_ViewClickedLocationOffset = { 0, 0 };
};
