#pragma once
#include "UIElementProperties.h"

struct ElementCodeProperties
{
	std::string name;
	std::string visibility;
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
};
