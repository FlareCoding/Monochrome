#pragma once
#include <Monochrome.h>
using namespace mc;

class LabelProperties
{
public:
	void Initialize(Ref<UIView> ElementPreviewArea, Ref<UIView> PropertiesView);

	void Open(Ref<UILabel> target);

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

	Ref<UITextbox> m_TextInput;
	Ref<UITextbox> m_FontInput;
	Ref<UITextbox> m_FontSizeInput;

	Ref<UITextbox> m_ColorRInput;
	Ref<UITextbox> m_ColorGInput;
	Ref<UITextbox> m_ColorBInput;
	Ref<UITextbox> m_ColorAInput;
};
