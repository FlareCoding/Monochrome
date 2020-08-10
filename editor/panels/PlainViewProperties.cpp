#include "PlainViewProperties.h"

void PlainViewProperties::InitPropertiesUI()
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
	m_ColorInput = AddInputField("Color: ", Position{ 100, 220 }, [this](Ref<UITextbox> src) { AssignColorFromField(m_TargetElement->layer.color, src); });

	// Corner Radius Label
	m_CornerRadiusLabel = MakeRef<UILabel>();
	m_CornerRadiusLabel->layer.frame = Frame(Position{ 450.0f / 2.0f - 80, 314 }, Size{ 200, 40 });
	m_CornerRadiusLabel->color = Color::white;
	m_CornerRadiusLabel->Text = "Corner Radius: ";
	m_CornerRadiusLabel->Properties.FontSize = 13;
	m_PropertiesPanel->AddSubview(m_CornerRadiusLabel);
	m_PropertiesList.push_back(m_CornerRadiusLabel);

	m_CornerRadiusSlider = MakeRef<UISlider>();
	m_CornerRadiusSlider->layer.frame = Frame(Position{ 450.0f / 2.0f - 100, 350 }, Size{ 250, 25 });
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

void PlainViewProperties::OnPropertiesOpened()
{
	m_PosXInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.x);
	m_PosYInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.position.y);
	m_WidthInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.width);
	m_HeightInput->Text = std::to_string((uint32_t)m_TargetElement->layer.frame.size.height);
	m_ColorInput->Text = utils::ColorToString(m_TargetElement->layer.color);
	m_CornerRadiusLabel->Text = "Corner Radius: " + std::to_string((uint32_t)m_TargetElement->CornerRadius);
}
