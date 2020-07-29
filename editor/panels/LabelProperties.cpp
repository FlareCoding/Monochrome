#include "LabelProperties.h"

void LabelProperties::Initialize(Ref<UIView> ElementPreviewArea, Ref<UIView> PropertiesView)
{
	m_ElementPreviewArea = ElementPreviewArea;
	m_PropertiesView = PropertiesView;

	InitPropertiesUI();
}

void LabelProperties::Open(Ref<UILabel> target)
{
	// Adding property views to the properties panel
	for (auto& view : m_PropertiesList)
		m_PropertiesView->AddSubview(view);

	// Setting current values
	m_PosXInput->Text = std::to_string((uint32_t)target->layer.frame.position.x);
	m_PosYInput->Text = std::to_string((uint32_t)target->layer.frame.position.y);
	m_WidthInput->Text = std::to_string((uint32_t)target->layer.frame.size.width);
	m_HeightInput->Text = std::to_string((uint32_t)target->layer.frame.size.height);

	m_TextInput->Text = target->Text;
	m_FontInput->Text = target->Properties.Font;
	m_FontSizeInput->Text = std::to_string(target->Properties.FontSize);

	m_ColorRInput->Text = std::to_string(target->color.r);
	m_ColorGInput->Text = std::to_string(target->color.g);
	m_ColorBInput->Text = std::to_string(target->color.b);
	m_ColorAInput->Text = std::to_string(target->color.alpha);
}

void LabelProperties::InitPropertiesUI()
{
	const auto AddInputField = [this](std::string title, Position position, std::function<void(Ref<UITextbox>, Ref<UILabel>)> input_handler, uint32_t font_size = 16) -> Ref<UITextbox> {
		Ref<UILabel> label = MakeRef<UILabel>();
		label->layer.frame = Frame(position, Size{ 120, 40 });
		label->color = Color::white;
		label->Text = title;
		label->Properties.FontSize = font_size;
		m_PropertiesView->AddSubview(label);

		Ref<UITextbox> Input = MakeRef<UITextbox>();
		Input->layer.frame = Frame(position + Position{ 110, 5 }, Size{ 200, 30 });
		Input->layer.color = Color(58, 58, 59, 1.0f);
		Input->TextColor = Color::white;
		Input->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
		Input->TextProperties.FontSize = font_size;
		Input->Placeholder = "Enter Value";
		Input->AddEventHandler<EventType::KeyPressed>([this, Input, input_handler](Event& e, UIView* sender) -> bool {
			if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			{
				Ref<UILabel> label = std::dynamic_pointer_cast<UILabel>(m_ElementPreviewArea->subviews.at(0));
				input_handler(Input, label);
			}
			return EVENT_HANDLED;
			});
		m_PropertiesView->AddSubview(Input);

		m_PropertiesList.push_back(label);
		m_PropertiesList.push_back(Input);

		return Input;
	};
	const auto GetTargetLabel = [this]() -> Ref<UILabel> {
		return std::dynamic_pointer_cast<UILabel>(m_ElementPreviewArea->subviews.at(0));
	};

	// Position X
	m_PosXInput = AddInputField("Pos.X: ", Position{ 100, 60 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
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
	m_PosYInput = AddInputField("Pos.Y: ", Position{ 100, 100 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
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
	m_WidthInput = AddInputField("Width: ", Position{ 100, 160 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
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
	m_HeightInput = AddInputField("Height: ", Position{ 100, 200 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			float val = std::stof(src->Text);
			target->layer.frame.size.height = val;
		}
		catch (...) 
		{
			src->Text = std::to_string((uint32_t)target->layer.frame.size.height);
		}
	});

	// Text
	m_TextInput = AddInputField("Text: ", Position{ 540, 60 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		target->Text = src->Text;
	});

	// Font
	m_FontInput = AddInputField("Font: ", Position{ 540, 100 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		target->Properties.Font = src->Text;
	});

	// Font Size
	m_FontSizeInput = AddInputField("Font Size: ", Position{ 540, 140 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->Properties.FontSize = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->Properties.FontSize);
		}
	});

	// Color RGB Label
	Ref<UILabel> label = MakeRef<UILabel>();
	label->layer.frame = Frame(Position{ 540, 180 }, Size{ 120, 40 });
	label->color = Color::white;
	label->Text = "Text Color";
	label->Properties.FontSize = 16;
	m_PropertiesView->AddSubview(label);
	m_PropertiesList.push_back(label);

	// ColorR
	m_ColorRInput = AddInputField("r: ", Position{ 590, 220 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->color.r = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->color.r);
		}
	}, 14);

	// ColorG
	m_ColorGInput = AddInputField("g: ", Position{ 590, 260 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->color.g = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->color.g);
		}
	}, 14);

	// ColorB
	m_ColorBInput = AddInputField("b: ", Position{ 590, 300 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			uint32_t val = std::stoi(src->Text);
			target->color.b = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->color.b);
		}
	}, 14);

	// ColorA
	m_ColorAInput = AddInputField("a: ", Position{ 590, 340 }, [](Ref<UITextbox> src, Ref<UILabel> target) {
		try {
			float val = std::stof(src->Text);
			target->color.alpha = val;
		}
		catch (...)
		{
			src->Text = std::to_string(target->color.alpha);
		}
	}, 14);
}
