#pragma once
#include "../utils/EditorUtils.h"

template <typename UIElement>
class UIElementProperties
{
public:
	void Initialize(Ref<UIView> PropertiesPanel);
	void Open(Ref<UIElement> target);

protected:
	Ref<UIView> m_PropertiesPanel = nullptr;
	Ref<UIElement> m_TargetElement = nullptr;

	std::vector<Ref<UIView>> m_PropertiesList;

protected:
	Ref<UITextbox> AddInputField(std::string title, Position position, std::function<void(Ref<UITextbox>)> input_handler, uint32_t font_size = 14);

	void AssignFloatFromField(float& target, Ref<UITextbox>& src);
	void AssignIntFromField(int& target, Ref<UITextbox>& src);
	void AssignColorFromField(Color& target, Ref<UITextbox>& src);

private:
	virtual void InitPropertiesUI() = 0;
	virtual void OnPropertiesOpened() = 0;
};

template<typename UIElement>
inline void UIElementProperties<UIElement>::Initialize(Ref<UIView> PropertiesPanel)
{
	m_PropertiesPanel = PropertiesPanel;
	InitPropertiesUI();
}

template<typename UIElement>
inline void UIElementProperties<UIElement>::Open(Ref<UIElement> target)
{
	m_TargetElement = target;

	// Adding property views to the properties panel
	for (auto& view : m_PropertiesList)
		m_PropertiesPanel->AddSubview(view);

	OnPropertiesOpened();
}

template<typename UIElement>
inline void UIElementProperties<UIElement>::AssignFloatFromField(float& target, Ref<UITextbox>& src)
{
	try 
	{
		float val = std::stof(src->Text);
		target = val;
	}
	catch (...)
	{
		src->Text = std::to_string((uint32_t)target);
	}
}

template<typename UIElement>
inline void UIElementProperties<UIElement>::AssignIntFromField(int& target, Ref<UITextbox>& src)
{
	try
	{
		int val = std::stoi(src->Text);
		target = val;
	}
	catch (...)
	{
		src->Text = std::to_string(target);
	}
}

template<typename UIElement>
inline void UIElementProperties<UIElement>::AssignColorFromField(Color& target, Ref<UITextbox>& src)
{
	try {
		auto color = utils::StringToColor(src->Text);
		target = color;
	}
	catch (...) { src->Text = utils::ColorToString(target); }
}

template<typename UIElement>
inline Ref<UITextbox> UIElementProperties<UIElement>::AddInputField(std::string title, Position position, std::function<void(Ref<UITextbox>)> input_handler, uint32_t font_size)
{
	Ref<UILabel> label = MakeRef<UILabel>();
	label->layer.frame = Frame(position, Size{ 120, 30 });
	label->color = Color::white;
	label->Text = title;
	label->Properties.FontSize = font_size;
	m_PropertiesPanel->AddSubview(label);

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
			input_handler(Input);
		}
		return EVENT_HANDLED;
		});
	m_PropertiesPanel->AddSubview(Input);

	m_PropertiesList.push_back(label);
	m_PropertiesList.push_back(Input);

	return Input;
}
