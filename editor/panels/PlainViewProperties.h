#pragma once
#include "UIElementProperties.h"

class PlainViewProperties : public UIElementProperties<UIView>
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

	Ref<UILabel> m_CornerRadiusLabel;
	Ref<UISlider> m_CornerRadiusSlider;
};
