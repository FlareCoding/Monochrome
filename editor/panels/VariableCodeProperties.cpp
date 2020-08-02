#include "VariableCodeProperties.h"

void VariableCodeProperties::InitPropertiesUI()
{
	// Variable Visibility Label
	Ref<UILabel> VariableVisibilityLabel = MakeRef<UILabel>();
	VariableVisibilityLabel->layer.frame = Frame(Position{ 50, 40 }, Size{ 180, 40 });
	VariableVisibilityLabel->color = Color::white;
	VariableVisibilityLabel->Text = "Variable Visibility";
	VariableVisibilityLabel->Properties.FontSize = 14;
	m_PropertiesList.push_back(VariableVisibilityLabel);

	std::vector<std::string> items = { "public", "private", "local" };
	m_VariableVisibilityCombobox = MakeRef<UICombobox>();
	m_VariableVisibilityCombobox->layer.frame = Frame(Position{ 50, 80 }, Size{ 180, 60 });
	m_VariableVisibilityCombobox->SetItems(items);
	m_VariableVisibilityCombobox->SelectItem("local");
	m_VariableVisibilityCombobox->SetItemBackgroundColor(Color(58, 58, 59, 1));
	m_VariableVisibilityCombobox->SetItemTextColor(Color::white);
	m_VariableVisibilityCombobox->layer.color = Color(62, 62, 63, 1);
	m_VariableVisibilityCombobox->SetDropdownArrowColor(Color::white);
	m_VariableVisibilityCombobox->SetSelectedItemColor(Color::white);
	m_VariableVisibilityCombobox->AddItemChangedEventHandler([this](size_t index, UICombobox* sender) {
		auto visibility = m_VariableVisibilityCombobox->GetItem(index);

		m_VariableNameInput->Visible = !visibility._Equal("local");
		m_VariableNameLabel->Visible = !visibility._Equal("local");
		
		if (visibility._Equal("local"))
			m_VariableNameInput->Text = "";

		if (m_RegisteredElementProperties.find(m_TargetElement) != m_RegisteredElementProperties.end())
		{
			m_RegisteredElementProperties[m_TargetElement].visibility = visibility;
			m_RegisteredElementProperties[m_TargetElement].name = m_VariableNameInput->Text;
		}
	});
	m_PropertiesList.push_back(m_VariableVisibilityCombobox);

	// Variable Name
	m_VariableNameLabel = MakeRef<UILabel>();
	m_VariableNameLabel->layer.frame = Frame({ 20, 150 }, Size{ 120, 30 });
	m_VariableNameLabel->color = Color::white;
	m_VariableNameLabel->Text = "Variable Name: ";
	m_VariableNameLabel->Properties.FontSize = 14;
	m_VariableNameLabel->Visible = false;
	m_PropertiesList.push_back(m_VariableNameLabel);

	m_VariableNameInput = MakeRef<UITextbox>();
	m_VariableNameInput->layer.frame = Frame(Position{ 140, 155 }, Size{ 140, 20 });
	m_VariableNameInput->layer.color = Color(58, 58, 59, 1.0f);
	m_VariableNameInput->TextColor = Color::white;
	m_VariableNameInput->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_VariableNameInput->TextProperties.FontSize = 14;
	m_VariableNameInput->Placeholder = "Enter Value";
	m_VariableNameInput->Visible = false;
	m_VariableNameInput->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) -> bool {
		if (m_RegisteredElementProperties.find(m_TargetElement) != m_RegisteredElementProperties.end())
			m_RegisteredElementProperties[m_TargetElement].name = m_VariableNameInput->Text;

		return EVENT_HANDLED;
	});
	m_PropertiesList.push_back(m_VariableNameInput);
}

void VariableCodeProperties::OnPropertiesOpened()
{
	if (m_RegisteredElementProperties.find(m_TargetElement) != m_RegisteredElementProperties.end())
	{
		// Existing Element
		auto& props = m_RegisteredElementProperties[m_TargetElement];
		
		m_VariableVisibilityCombobox->SelectItem(props.visibility);
		m_VariableNameInput->Text = props.name;
	}
	else
	{
		// New Element
		m_VariableVisibilityCombobox->SelectItem("local");
		RegisterElement(m_TargetElement);
	}
}

void VariableCodeProperties::RegisterElement(Ref<UIView> target)
{
	if (m_RegisteredElementProperties.find(target) != m_RegisteredElementProperties.end()) return;

	ElementCodeProperties props;
	props.visibility = "local";
	props.name = "";

	m_RegisteredElementProperties[target] = props;
}

void VariableCodeProperties::UnregisterElement(Ref<UIView> target)
{
	if (m_RegisteredElementProperties.find(target) != m_RegisteredElementProperties.end())
	{
		m_RegisteredElementProperties.erase(target);
	}
}
