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
	Ref<ProjectView> m_ProjectView;

	Ref<UIScrollPanel> m_Toolbox;
	Ref<UIScrollPanel> m_PropertiesPanel;
};
