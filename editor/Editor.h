#pragma once
#include "panels/LabelProperties.h"
#include "panels/ButtonProperties.h"
#include "panels/VariableCodeProperties.h"
#include "utils/ProjectGenerator.h"

class MonochromeEditor
{
public:
	void Start();
	void Initialize();

private:
	void CreateEditorWindow();
	void InitEditorUI();

private:
	Ref<UIWindow> m_EditorWindow;

	uint32_t m_EditorWidth = 1820;
	uint32_t m_EditorHeight = 980;

private:
	// Element Editing
	Ref<UIView> m_ElementPreviewArea;
	Ref<UIScrollPanel> m_Toolbox;

private:
	// Element Properties
	Ref<UIView> m_PropertiesView;

	LabelProperties m_LabelProperties;
	ButtonProperties m_ButtonProperties;
	VariableCodeProperties m_VariableCodeProperties;

	Ref<UIButton> m_OpenVariablePropertiesButton;

	void OpenElementProperties(Ref<UIView> TargetElement);

private:
	// Project Window Settings
	uint32_t m_ProjectWindowWidth = 0;
	uint32_t m_ProjectWindowHeight = 0;

	Ref<UITextbox> m_ProjectWindowWidthTextbox;
	Ref<UITextbox> m_ProjectWindowHeightTextbox;
	Ref<UITextbox> m_ProjectWindowTitleTextbox;
	Ref<UITextbox> m_ProjectWindowColorTextbox;

	Ref<UIButton> m_OpenProjectWindowButton;

private:
	// Project Window Functionality
	Ref<UIWindow> m_ProjectWindow;
	Ref<UIButton> m_AddElementToProjectWindowButton;

	std::vector<Ref<UIView>> m_ProjectUIElements;

	void OpenProjectWindow();
	void AddElementToProjectWindow();

private:
	// Project Application Saving
	Ref<UITextbox> m_ProjectNameTextbox;
	Ref<UITextbox> m_ProjectPathTextbox;
	Ref<UITextbox> m_UIClassNameTextbox;
	Ref<UIButton> m_SelectProjectPathButton;
	Ref<UIButton> m_GenerateProjectButton;

	// Monochrome Source Path
	Ref<UITextbox> m_MonochromeSourcePathTextbox;
	Ref<UIButton> m_SelectMonochromeSourcePathButton;

	// Monochrome Library Debug Path
	Ref<UITextbox> m_MonochromeLibDbgPathTextbox;
	Ref<UIButton> m_SelectMonochromeLibDbgPathButton;

	// Monochrome Library Release Path
	Ref<UITextbox> m_MonochromeLibRelPathTextbox;
	Ref<UIButton> m_SelectMonochromeLibRelPathButton;

	void GenerateProjectSolution();
};
