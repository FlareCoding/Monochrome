#include "ButtonProperties.h"

void ButtonProperties::InitPropertiesUI()
{
	// Position X
	m_PosXInput = AddInputField("Pos.X: ", Position{ 100, 60 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.position.x, src); });

	// Position Y
	m_PosYInput = AddInputField("Pos.Y: ", Position{ 100, 90 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.position.y, src); });

	// Width
	m_WidthInput = AddInputField("Width: ", Position{ 100, 140 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.size.width, src); });

	// Height
	m_HeightInput = AddInputField("Height: ", Position{ 100, 170 }, [this](Ref<UITextbox> src) { AssignFloatFromField(m_TargetElement->layer.frame.size.height, src); });

	// Button Color Tex Label
	Ref<UILabel> ButtonColorLabel = MakeRef<UILabel>();
	ButtonColorLabel->layer.frame = Frame(Position{ 80, 210 }, Size{ 160, 40 });
	ButtonColorLabel->color = Color::white;
	ButtonColorLabel->Text = "Button Color";
	ButtonColorLabel->Properties.FontSize = 14;
	m_PropertiesPanel->AddSubview(ButtonColorLabel);
	m_PropertiesList.push_back(ButtonColorLabel);

	// Button Color
	m_ButtonColorInput = AddInputField("Normal: ", Position{ 100, 250 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->layer.color, src); }, 12);

	// Button Hover On Color
	m_HoverOnColorInput = AddInputField("OnHover: ", Position{ 100, 280 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->HoverOnColor, src); }, 12);

	// Button On Press Color
	m_MousePressedColorInput = AddInputField("OnPress: ", Position{ 100, 310 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->OnMousePressColor, src); }, 12);

	// Text
	m_TextInput = AddInputField("Text: ", Position{ 540, 60 }, [this](Ref<UITextbox> src) { m_TargetElement->Label->Text = src->Text; });

	// Font
	m_FontInput = AddInputField("Font: ", Position{ 540, 90 }, [this](Ref<UITextbox> src) { m_TargetElement->Label->Properties.Font = src->Text; });

	// Font Size
	m_FontSizeInput = AddInputField("Font Size: ", Position{ 540, 120 }, [this](Ref<UITextbox> src) { AssignIntFromField((int&)m_TargetElement->Label->Properties.FontSize, src); });

	// Text Color
	m_TextColorInput = AddInputField("Text Color: ", Position{ 540, 150 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->Label->color, src); });

	// Corner Radius Label
	m_CornerRadiusLabel = MakeRef<UILabel>();
	m_CornerRadiusLabel->layer.frame = Frame(Position{ 450.0f / 2.0f - 80, 354 }, Size{ 200, 40 });
	m_CornerRadiusLabel->color = Color::white;
	m_CornerRadiusLabel->Text = "Corner Radius: ";
	m_CornerRadiusLabel->Properties.FontSize = 13;
	m_PropertiesPanel->AddSubview(m_CornerRadiusLabel);
	m_PropertiesList.push_back(m_CornerRadiusLabel);

	m_CornerRadiusSlider = MakeRef<UISlider>();
	m_CornerRadiusSlider->layer.frame = Frame(Position{ 450.0f / 2.0f - 100, 390 }, Size{ 250, 25 });
	m_CornerRadiusSlider->SliderKnobShape = Shape::Circle;
	m_CornerRadiusSlider->VisibleTickmarks = false;
	m_CornerRadiusSlider->MaxValue = 40;
	m_CornerRadiusSlider->Intervals = 1;
	m_CornerRadiusSlider->Value = 6;
	m_CornerRadiusSlider->AddValueChangedEventHandler([this](float value, UISlider* sender) {
		m_TargetElement->CornerRadius = value;
		m_CornerRadiusLabel->Text = "Corner Radius: " + std::to_string((uint32_t)m_TargetElement->CornerRadius);
	});

	m_PropertiesPanel->AddSubview(m_CornerRadiusSlider);
	m_PropertiesList.push_back(m_CornerRadiusSlider);
}

void ButtonProperties::OnPropertiesOpened()
{
	m_PosXInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.x);
	m_PosYInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.y);
	m_WidthInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.width);
	m_HeightInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.height);

	m_ButtonColorInput->Text = utils::ColorToString(m_TargetElement->layer.color);
	m_HoverOnColorInput->Text = utils::ColorToString(m_TargetElement->HoverOnColor);
	m_MousePressedColorInput->Text = utils::ColorToString(m_TargetElement->OnMousePressColor);

	m_TextInput->Text = m_TargetElement->Label->Text;
	m_FontInput->Text = m_TargetElement->Label->Properties.Font;
	m_FontSizeInput->Text = std::to_string(m_TargetElement->Label->Properties.FontSize);

	m_TextColorInput->Text = utils::ColorToString(m_TargetElement->Label->color);

	m_CornerRadiusLabel->Text = "Corner Radius: " + std::to_string((uint32_t)m_TargetElement->CornerRadius);
}
