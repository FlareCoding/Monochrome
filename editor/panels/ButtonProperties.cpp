#include "ButtonProperties.h"

void ButtonProperties::Initialize(Ref<UIView> ElementPreviewArea, Ref<UIView> PropertiesView)
{
	m_ElementPreviewArea = ElementPreviewArea;
	m_PropertiesView = PropertiesView;

	InitPropertiesUI();
}

void ButtonProperties::Open(Ref<UIButton> target)
{
	for (auto& view : m_PropertiesList)
		m_PropertiesView->AddSubview(view);

	m_PosXInput->Text = std::to_string((uint32_t)target->layer.frame.position.x);
	m_PosYInput->Text = std::to_string((uint32_t)target->layer.frame.position.y);
	m_WidthInput->Text = std::to_string((uint32_t)target->layer.frame.size.width);
	m_HeightInput->Text = std::to_string((uint32_t)target->layer.frame.size.height);

	m_ButtonColorInput->Text = utils::ColorToString(target->layer.color);
	m_HoverOnColorInput->Text = utils::ColorToString(target->HoverOnColor);
	m_MousePressedColorInput->Text = utils::ColorToString(target->OnMousePressColor);

	m_TextInput->Text = target->Label->Text;
	m_FontInput->Text = target->Label->Properties.Font;
	m_FontSizeInput->Text = std::to_string(target->Label->Properties.FontSize);

	m_TextColorRInput->Text = std::to_string(target->Label->color.r);
	m_TextColorGInput->Text = std::to_string(target->Label->color.g);
	m_TextColorBInput->Text = std::to_string(target->Label->color.b);
	m_TextColorAInput->Text = std::to_string(target->Label->color.alpha);

	m_CornerRadiusLabel->Text = "Corner Radius: " + std::to_string((uint32_t)target->CornerRadius);
}

void ButtonProperties::InitPropertiesUI()
{
	const auto AddInputField = [this](std::string title, Position position, std::function<void(Ref<UITextbox>, Ref<UIButton>)> input_handler, uint32_t font_size = 14) -> Ref<UITextbox> {
		Ref<UILabel> label = MakeRef<UILabel>();
		label->layer.frame = Frame(position, Size{ 120, 30 });
		label->color = Color::white;
		label->Text = title;
		label->Properties.FontSize = font_size;
		m_PropertiesView->AddSubview(label);

		Ref<UITextbox> Input = MakeRef<UITextbox>();
		Input->layer.frame = Frame(position + Position{ 110, 5 }, Size{ 150, 20 });
		Input->layer.color = Color(58, 58, 59, 1.0f);
		Input->TextColor = Color::white;
		Input->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
		Input->TextProperties.FontSize = font_size;
		Input->Placeholder = "Enter Value";
		Input->AddEventHandler<EventType::KeyPressed>([this, Input, input_handler](Event& e, UIView* sender) -> bool {
			if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			{
				Ref<UIButton> button = std::dynamic_pointer_cast<UIButton>(m_ElementPreviewArea->subviews.at(0));
				input_handler(Input, button);
			}
			return EVENT_HANDLED;
		});
		m_PropertiesView->AddSubview(Input);

		m_PropertiesList.push_back(label);
		m_PropertiesList.push_back(Input);

		return Input;
	};
	const auto GetTargetButton = [this]() -> Ref<UIButton> {
		return std::dynamic_pointer_cast<UIButton>(m_ElementPreviewArea->subviews.at(0));
	};

	// Position X
	m_PosXInput = AddInputField("Pos.X: ", Position{ 100, 60 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			float val = std::stof(src->Text);
			target->layer.frame.position.x = val;
		}
		catch (...)
		{
			src->Text = std::to_string((uint32_t)target->layer.frame.position.x);
		}
	});

	// Position Y
	m_PosYInput = AddInputField("Pos.Y: ", Position{ 100, 90 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			float val = std::stof(src->Text);
			target->layer.frame.position.y = val;
		}
		catch (...)
		{
			src->Text = std::to_string((uint32_t)target->layer.frame.position.y);
		}
	});

	// Width
	m_WidthInput = AddInputField("Width: ", Position{ 100, 140 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			float val = std::stof(src->Text);
			target->layer.frame.size.width = val;
		}
		catch (...)
		{
			src->Text = std::to_string((uint32_t)target->layer.frame.size.width);
		}
	});

	// Height
	m_HeightInput = AddInputField("Height: ", Position{ 100, 170 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			float val = std::stof(src->Text);
			target->layer.frame.size.height = val;
		}
		catch (...)
		{
			src->Text = std::to_string((uint32_t)target->layer.frame.size.height);
		}
	});

	// Button Color Tex Label
	Ref<UILabel> ButtonColorLabel = MakeRef<UILabel>();
	ButtonColorLabel->layer.frame = Frame(Position{ 80, 210 }, Size{ 160, 40 });
	ButtonColorLabel->color = Color::white;
	ButtonColorLabel->Text = "Button Color";
	ButtonColorLabel->Properties.FontSize = 14;
	m_PropertiesView->AddSubview(ButtonColorLabel);
	m_PropertiesList.push_back(ButtonColorLabel);

	// Button Color
	m_ButtonColorInput = AddInputField("Normal: ", Position{ 100, 250 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			auto color = utils::StringToColor(src->Text);
			target->layer.color = color;
		}
		catch (...) { src->Text = utils::ColorToString(target->layer.color); }
	}, 12);

	// Button Hover On Color
	m_HoverOnColorInput = AddInputField("OnHover: ", Position{ 100, 280 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			auto color = utils::StringToColor(src->Text);
			target->HoverOnColor = color;
		}
		catch (...) { src->Text = utils::ColorToString(target->HoverOnColor); }
	}, 12);

	// Button On Press Color
	m_MousePressedColorInput = AddInputField("OnPress: ", Position{ 100, 310 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			auto color = utils::StringToColor(src->Text);
			target->OnMousePressColor = color;
		}
		catch (...) { src->Text = utils::ColorToString(target->OnMousePressColor); }
	}, 12);

	// Text
	m_TextInput = AddInputField("Text: ", Position{ 540, 60 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		target->Label->Text = src->Text;
	});

	// Font
	m_FontInput = AddInputField("Font: ", Position{ 540, 90 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		target->Label->Properties.Font = src->Text;
	});

	// Font Size
	m_FontSizeInput = AddInputField("Font Size: ", Position{ 540, 120 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->Label->Properties.FontSize = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Label->Properties.FontSize);
		}
	});

	// Text Color RGB Label
	Ref<UILabel> TextColorLabel = MakeRef<UILabel>();
	TextColorLabel->layer.frame = Frame(Position{ 540, 160 }, Size{ 120, 40 });
	TextColorLabel->color = Color::white;
	TextColorLabel->Text = "Text Color";
	TextColorLabel->Properties.FontSize = 14;
	m_PropertiesView->AddSubview(TextColorLabel);
	m_PropertiesList.push_back(TextColorLabel);

	// ColorR
	m_TextColorRInput = AddInputField("r: ", Position{ 590, 190 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->Label->color.r = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Label->color.r);
		}
	}, 12);

	// ColorG
	m_TextColorGInput = AddInputField("g: ", Position{ 590, 220 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->Label->color.g = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Label->color.g);
		}
	}, 12);

	// ColorB
	m_TextColorBInput = AddInputField("b: ", Position{ 590, 250 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->Label->color.b = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Label->color.b);
		}
	}, 12);

	// ColorA
	m_TextColorAInput = AddInputField("a: ", Position{ 590, 280 }, [](Ref<UITextbox> src, Ref<UIButton> target) {
		try {
			float val = std::stof(src->Text);
			target->Label->color.alpha = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Label->color.alpha);
		}
	}, 12);


	// Text Color RGB Label
	m_CornerRadiusLabel = MakeRef<UILabel>();
	m_CornerRadiusLabel->layer.frame = Frame(Position{ m_ElementPreviewArea->layer.frame.size.width / 2.0f - 50, 340 }, Size{ 200, 40 });
	m_CornerRadiusLabel->color = Color::white;
	m_CornerRadiusLabel->Text = "Corner Radius: ";
	m_CornerRadiusLabel->Properties.FontSize = 14;
	m_PropertiesView->AddSubview(m_CornerRadiusLabel);
	m_PropertiesList.push_back(m_CornerRadiusLabel);

	m_CornerRadiusSlider = MakeRef<UISlider>();
	m_CornerRadiusSlider->layer.frame = Frame(Position{ m_ElementPreviewArea->layer.frame.size.width / 2.0f - 80, 380 }, Size{ 250, 25 });
	m_CornerRadiusSlider->SliderKnobShape = Shape::Circle;
	m_CornerRadiusSlider->VisibleTickmarks = false;
	m_CornerRadiusSlider->MaxValue = 40;
	m_CornerRadiusSlider->Intervals = 1;
	m_CornerRadiusSlider->Value = 6;
	m_CornerRadiusSlider->AddValueChangedEventHandler([this, GetTargetButton](float value, UISlider* sender) {
		GetTargetButton()->CornerRadius = value;
		m_CornerRadiusLabel->Text = "Corner Radius: " + std::to_string((uint32_t)GetTargetButton()->CornerRadius);
	});

	m_PropertiesView->AddSubview(m_CornerRadiusSlider);
	m_PropertiesList.push_back(m_CornerRadiusSlider);
}
