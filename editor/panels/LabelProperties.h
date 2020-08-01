#pragma once
#include "UIElementProperties.h"

class LabelProperties : public UIElementProperties<UILabel>
{
private:
	// Inherited via UIElementProperties
	virtual void InitPropertiesUI() override;

	// Inherited via UIElementProperties
	virtual void OnPropertiesOpened() override;

private:
	Ref<UITextbox> m_PosXInput;
	Ref<UITextbox> m_PosYInput;
	Ref<UITextbox> m_WidthInput;
	Ref<UITextbox> m_HeightInput;
	Ref<UITextbox> m_ColorInput;

	Ref<UITextbox> m_TextInput;
	Ref<UITextbox> m_FontInput;
	Ref<UITextbox> m_FontSizeInput;
};
