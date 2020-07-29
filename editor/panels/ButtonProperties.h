#pragma once
#include "../utils/EditorUtils.h"

class ButtonProperties
{
public:
	void Initialize(Ref<UIView> ElementPreviewArea, Ref<UIView> PropertiesView);

	void Open(Ref<UIButton> target);

private:
	Ref<UIView> m_ElementPreviewArea;

private:
	Ref<UIView> m_PropertiesView;
	void InitPropertiesUI();

	std::vector<Ref<UIView>> m_PropertiesList;

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

	Ref<UITextbox> m_TextColorRInput;
	Ref<UITextbox> m_TextColorGInput;
	Ref<UITextbox> m_TextColorBInput;
	Ref<UITextbox> m_TextColorAInput;

	Ref<UILabel> m_CornerRadiusLabel;
	Ref<UISlider> m_CornerRadiusSlider;
};
