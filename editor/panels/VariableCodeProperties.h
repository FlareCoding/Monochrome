#pragma once
#include "UIElementProperties.h"

struct ElementEventHandlerData
{
	bool generateClassFunction = false;
	std::string classFunctionName = "";
	std::string classFunctionVisibility = "";
	std::string returnStatus = "Handled";
};

struct ElementCodeProperties
{
	std::string name;
	std::string visibility;
	std::map<std::string, ElementEventHandlerData> eventHandlerDataMap;
};

class VariableCodeProperties : public UIElementProperties<UIView>
{
public:
	void RegisterElement(Ref<UIView> target);
	void UnregisterElement(Ref<UIView> target);

	std::map<Ref<UIView>, ElementCodeProperties>* GetRegisteredElementCodeProperties() { return &m_RegisteredElementProperties; }

private:
	std::map<Ref<UIView>, ElementCodeProperties> m_RegisteredElementProperties;

private:
	// Inherited via UIElementProperties
	virtual void InitPropertiesUI() override;

	// Inherited via UIElementProperties
	virtual void OnPropertiesOpened() override;

private:
	Ref<UICombobox> m_VariableVisibilityCombobox;

	Ref<UILabel> m_VariableNameLabel;
	Ref<UITextbox> m_VariableNameInput;

	Ref<UIScrollPanel> m_EventHandlersPanel;
	Ref<UILabel> m_EventTypeSelectedLabel;
	Ref<UICheckbox> m_UseEventHandlerCheckbox;

	Ref<UICheckbox> m_GenerateClassFunctionCheckbox;
	Ref<UICombobox> m_ClassFunctionVisbilityCombobox;
	Ref<UITextbox>  m_ClassFunctionNameInput;

	Ref<UILabel> m_EventReturnStatusLabel;
	Ref<UICombobox> m_EventReturnStatusCombobox;
	void OpenEventHandlerSettingsUI(const std::string& event_type);

private:
	std::vector<std::string> EventTypes =
	{
		"MouseButtonPressed",
		"MouseButtonClicked",
		"MouseMoved",
		"MouseHoverOn",
		"MouseHoverOff",
		"MouseScrolled",
		"KeyPressed",
		"KeyReleased",
		"FocusChanged",
	};

	size_t m_SelectedEventTypeIndex = 0;
};
