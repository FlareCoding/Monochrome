#include "LabelProperties.h"

void LabelProperties::InitPropertiesUI()
{
	// Position X
	m_PosXInput = AddInputField("Pos.X: ", Position{ 100, 60 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.position.x, src); });

	// Position Y
	m_PosYInput = AddInputField("Pos.Y: ", Position{ 100, 90 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.position.y, src); });

	// Width
	m_WidthInput = AddInputField("Width: ", Position{ 100, 140 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.size.width, src); });

	// Height
	m_HeightInput = AddInputField("Height: ", Position{ 100, 170 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.size.height, src); });

	// Color
	m_ColorInput = AddInputField("Color: ", Position{ 540, 150 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->color, src); });

	// Text
	m_TextInput = AddInputField("Text: ", Position{ 540, 60 }, [this](Ref<UITextbox> src) { m_TargetElement->Text = src->Text; });

	// Font
	m_FontInput = AddInputField("Font: ", Position{ 540, 90 }, [this](Ref<UITextbox> src) { m_TargetElement->Properties.Font = src->Text; });

	// Font Size
	m_FontSizeInput = AddInputField("Font Size: ", Position{ 540, 120 }, [this](Ref<UITextbox> src) { AssignIntFromField((int&)m_TargetElement->Properties.FontSize, src); });
}

void LabelProperties::OnPropertiesOpened()
{
	m_PosXInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.x);
	m_PosYInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.y);
	m_WidthInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.width);
	m_HeightInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.height);
	m_ColorInput->Text = utils::ColorToString(m_TargetElement->color);

	m_TextInput->Text = m_TargetElement->Text;
	m_FontInput->Text = m_TargetElement->Properties.Font;
	m_FontSizeInput->Text = std::to_string(m_TargetElement->Properties.FontSize);
}
