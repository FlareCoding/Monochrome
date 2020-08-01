#pragma once
#include "UIElementProperties.h"

class ButtonProperties : public UIElementProperties<UIButton>
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
	
	Ref<UITextbox> m_ButtonColorInput;
	Ref<UITextbox> m_HoverOnColorInput;
	Ref<UITextbox> m_MousePressedColorInput;
	
	Ref<UITextbox> m_TextInput;
	Ref<UITextbox> m_FontInput;
	Ref<UITextbox> m_FontSizeInput;
	
	Ref<UITextbox> m_TextColorInput;
	
	Ref<UILabel> m_CornerRadiusLabel;
	Ref<UISlider> m_CornerRadiusSlider;
};

