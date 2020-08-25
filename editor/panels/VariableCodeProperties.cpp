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

		m_VariableNameInput->Visible = (visibility != "local");
		m_VariableNameLabel->Visible = (visibility != "local");

		if (visibility == "local")
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
	m_VariableNameInput->textProperties.FontSize = 14;
	m_VariableNameInput->Placeholder = "Enter Value";
	m_VariableNameInput->Visible = false;
	m_VariableNameInput->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) -> bool {
		if (m_RegisteredElementProperties.find(m_TargetElement) != m_RegisteredElementProperties.end())
			m_RegisteredElementProperties[m_TargetElement].name = m_VariableNameInput->Text;

		return EVENT_HANDLED;
	});
	m_PropertiesList.push_back(m_VariableNameInput);

	Ref<UILabel> EventHandlersLabel = MakeRef<UILabel>();
	EventHandlersLabel->layer.frame = Frame(Position{ 350, 40 }, Size{ 180, 40 });
	EventHandlersLabel->color = Color::white;
	EventHandlersLabel->Text = "Event Handlers";
	EventHandlersLabel->Properties.FontSize = 14;
	m_PropertiesList.push_back(EventHandlersLabel);

	// Event Handlers Panel
	m_EventHandlersPanel = MakeRef<UIScrollPanel>();
	m_EventHandlersPanel->layer.frame = Frame(Position{ 320, 80 }, Size{ 240, 344 });
	m_EventHandlersPanel->layer.color = Color(60, 60, 61, 1);
	m_EventHandlersPanel->ContentView->layer.frame.size.height = m_EventHandlersPanel->layer.frame.size.height;
	m_EventHandlersPanel->ContentView->layer.color = m_EventHandlersPanel->layer.color;
	m_PropertiesList.push_back(m_EventHandlersPanel);

	Position EventTypeButtonPosition = { 2, 2 };
	for (size_t i = 0; i < EventTypes.size(); i++)
	{
		auto EventTypeButton = MakeRef<UIButton>();
		EventTypeButton->layer.frame = Frame(EventTypeButtonPosition, { m_EventHandlersPanel->layer.frame.size.width - 4, 36 });
		EventTypeButton->Label->Text = EventTypes[i];
		EventTypeButton->CornerRadius = 2;
		EventTypeButton->layer.color = Color(38, 38, 39, 1.0f);
		EventTypeButton->Label->color = Color(100, 0, 0, 1.0f);
		EventTypeButton->AddEventHandler<EventType::MouseButtonClicked>([this, i, EventTypeButton](Event& e, UIView* sender) -> bool {
			m_EventTypeSelectedLabel->Text = "Event Type Selected:  " + EventTypes[i];
			m_SelectedEventTypeIndex = i;
			m_UseEventHandlerCheckbox->Checked = (EventTypeButton->Label->color == Color(0, 140, 0, 1));
			OpenEventHandlerSettingsUI(EventTypes[i]);
			return EVENT_HANDLED;
		});
		m_EventHandlersPanel->AddChild(EventTypeButton);

		EventTypeButtonPosition += { 0, 38 };
	}

	m_EventTypeSelectedLabel = MakeRef<UILabel>();
	m_EventTypeSelectedLabel->layer.frame = Frame(Position{ 650, 80 }, Size{ 340, 40 });
	m_EventTypeSelectedLabel->color = Color::white;
	m_EventTypeSelectedLabel->Text = "Event Type Selected:  " + EventTypes[0];
	m_EventTypeSelectedLabel->Properties.FontSize = 14;
	m_EventTypeSelectedLabel->Properties.Allignment = TextAlignment::LEADING;
	m_EventTypeSelectedLabel->Properties.Style = TextStyle::BOLD;
	m_PropertiesList.push_back(m_EventTypeSelectedLabel);

	m_UseEventHandlerCheckbox = MakeRef<UICheckbox>();
	m_UseEventHandlerCheckbox->layer.frame = Frame(Position{ 700, 115 }, Size{ 240, 40 });
	m_UseEventHandlerCheckbox->Label->Text = "Use Event Handler";
	m_UseEventHandlerCheckbox->Label->Properties.FontSize = 12;
	m_UseEventHandlerCheckbox->BoxSize = 14;
	m_UseEventHandlerCheckbox->layer.color = Color(88, 88, 89, 1);
	m_UseEventHandlerCheckbox->CheckedBoxColor = Color(108, 108, 109, 1);
	m_UseEventHandlerCheckbox->CheckmarkColor = Color::white;
	m_UseEventHandlerCheckbox->AddValueChangedEventHandler([this](bool checked, UICheckbox* sender) {
		auto EventTypeButton = std::dynamic_pointer_cast<UIButton>(m_EventHandlersPanel->ContentView->subviews[m_SelectedEventTypeIndex]);
		EventTypeButton->Label->color = checked ? Color(0, 140, 0, 1) : Color(100, 0, 0, 1.0f);

		auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;
		std::string event_type = EventTypes[m_SelectedEventTypeIndex];

		if (checked)
		{
			if (HandlerMap.find(event_type) == HandlerMap.end())
			{
				ElementEventHandlerData data;
				data.classFunctionVisibility = "private";
				data.returnStatus = "Handled";
				HandlerMap[event_type] = data;
			}
		}
		else
		{
			if (HandlerMap.find(event_type) != HandlerMap.end())
				HandlerMap.erase(event_type);
		}

		OpenEventHandlerSettingsUI(EventTypeButton->Label->Text);
	});
	m_PropertiesList.push_back(m_UseEventHandlerCheckbox);

	m_GenerateClassFunctionCheckbox = MakeRef<UICheckbox>();
	m_GenerateClassFunctionCheckbox->layer.frame = Frame(Position{ 660, 190 }, Size{ 280, 40 });
	m_GenerateClassFunctionCheckbox->Label->Text = "Generate Class Member Function";
	m_GenerateClassFunctionCheckbox->Label->Properties.FontSize = 14;
	m_GenerateClassFunctionCheckbox->Label->Properties.Style = TextStyle::BOLD;
	m_GenerateClassFunctionCheckbox->BoxSize = 14;
	m_GenerateClassFunctionCheckbox->layer.color = Color(88, 88, 89, 1);
	m_GenerateClassFunctionCheckbox->CheckedBoxColor = Color(108, 108, 109, 1);
	m_GenerateClassFunctionCheckbox->CheckmarkColor = Color::white;
	m_GenerateClassFunctionCheckbox->Checked = true;
	m_GenerateClassFunctionCheckbox->Visible = false;
	m_GenerateClassFunctionCheckbox->AddValueChangedEventHandler([this](bool checked, UICheckbox* sender) {
		m_ClassFunctionVisbilityCombobox->Visible = checked;
		m_ClassFunctionNameInput->Visible = checked;

		auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;
		std::string event_type = EventTypes[m_SelectedEventTypeIndex];
		HandlerMap[event_type].generateClassFunction = checked;
	});
	m_PropertiesList.push_back(m_GenerateClassFunctionCheckbox);

	m_ClassFunctionVisbilityCombobox = MakeRef<UICombobox>();
	m_ClassFunctionVisbilityCombobox->layer.frame = Frame(Position{ 635, 240 }, Size{ 120, 60 });
	m_ClassFunctionVisbilityCombobox->SetItems({ "public", "private" });
	m_ClassFunctionVisbilityCombobox->SelectItem("private");
	m_ClassFunctionVisbilityCombobox->SetItemBackgroundColor(Color(58, 58, 59, 1));
	m_ClassFunctionVisbilityCombobox->SetItemTextColor(Color::white);
	m_ClassFunctionVisbilityCombobox->layer.color = Color(62, 62, 63, 1);
	m_ClassFunctionVisbilityCombobox->SetDropdownArrowColor(Color::white);
	m_ClassFunctionVisbilityCombobox->SetSelectedItemColor(Color::white);
	m_ClassFunctionVisbilityCombobox->Visible = false;
	m_ClassFunctionVisbilityCombobox->AddItemChangedEventHandler([this](size_t index, UICombobox* sender) {
		std::string event_type = EventTypes[m_SelectedEventTypeIndex];
		auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;

		HandlerMap[event_type].classFunctionVisibility = sender->GetSelectedItem();
	});
	m_PropertiesList.push_back(m_ClassFunctionVisbilityCombobox);

	m_ClassFunctionNameInput = MakeRef<UITextbox>();
	m_ClassFunctionNameInput->layer.frame = Frame(Position{ 775, 240 }, Size{ 170, 20 });
	m_ClassFunctionNameInput->layer.color = Color(58, 58, 59, 1.0f);
	m_ClassFunctionNameInput->TextColor = Color::white;
	m_ClassFunctionNameInput->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_ClassFunctionNameInput->textProperties.FontSize = 14;
	m_ClassFunctionNameInput->Placeholder = "Enter name";
	m_ClassFunctionNameInput->Visible = false;
	m_ClassFunctionNameInput->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) -> bool {
		auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;
		std::string event_type = EventTypes[m_SelectedEventTypeIndex];
		HandlerMap[event_type].classFunctionName = m_ClassFunctionNameInput->Text;
		return EVENT_HANDLED;
	});
	m_PropertiesList.push_back(m_ClassFunctionNameInput);

	m_EventReturnStatusLabel = MakeRef<UILabel>();
	m_EventReturnStatusLabel->layer.frame = Frame(Position{ 620, 300 }, Size{ 180, 40 });
	m_EventReturnStatusLabel->color = Color::white;
	m_EventReturnStatusLabel->Text = "Return Status";
	m_EventReturnStatusLabel->Properties.FontSize = 14;
	m_EventReturnStatusLabel->Properties.Style = TextStyle::BOLD;
	m_EventReturnStatusLabel->Visible = false;
	m_PropertiesList.push_back(m_EventReturnStatusLabel);

	m_EventReturnStatusCombobox = MakeRef<UICombobox>();
	m_EventReturnStatusCombobox->layer.frame = Frame(Position{ 770, 310 }, Size{ 120, 60 });
	m_EventReturnStatusCombobox->SetItems({ "Handled", "Unhandled" });
	m_EventReturnStatusCombobox->SelectItem("Handled");
	m_EventReturnStatusCombobox->SetItemBackgroundColor(Color(58, 58, 59, 1));
	m_EventReturnStatusCombobox->SetItemTextColor(Color::white);
	m_EventReturnStatusCombobox->layer.color = Color(62, 62, 63, 1);
	m_EventReturnStatusCombobox->SetDropdownArrowColor(Color::white);
	m_EventReturnStatusCombobox->SetSelectedItemColor(Color::white);
	m_EventReturnStatusCombobox->Visible = false;
	m_EventReturnStatusCombobox->AddItemChangedEventHandler([this](size_t index, UICombobox* sender) {
		auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;
		std::string event_type = EventTypes[m_SelectedEventTypeIndex];
		HandlerMap[event_type].returnStatus = sender->GetSelectedItem();
	});
	m_PropertiesList.push_back(m_EventReturnStatusCombobox);
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

	OpenEventHandlerSettingsUI(EventTypes[m_SelectedEventTypeIndex]);
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

void VariableCodeProperties::OpenEventHandlerSettingsUI(const std::string& event_type)
{
	m_EventTypeSelectedLabel->Text = "Event Type Selected:  " + event_type;
	auto& HandlerMap = m_RegisteredElementProperties[m_TargetElement].eventHandlerDataMap;

	m_UseEventHandlerCheckbox->Checked = (HandlerMap.find(event_type) != HandlerMap.end());
	if (m_UseEventHandlerCheckbox->Checked)
	{
		auto& data = HandlerMap[event_type];

		// existing event handler data
		m_GenerateClassFunctionCheckbox->Checked = data.generateClassFunction;
		m_ClassFunctionNameInput->Text = data.classFunctionName;
		m_ClassFunctionVisbilityCombobox->SelectItem(data.classFunctionVisibility);
		m_EventReturnStatusCombobox->SelectItem(data.returnStatus);

		m_GenerateClassFunctionCheckbox->Visible = true;

		if (HandlerMap[event_type].generateClassFunction)
		{
			m_ClassFunctionVisbilityCombobox->Visible = true;
			m_ClassFunctionNameInput->Visible = true;
		}

		m_EventReturnStatusLabel->Visible = true;
		m_EventReturnStatusCombobox->Visible = true;
	}
	else
	{
		m_GenerateClassFunctionCheckbox->Visible = false;
		m_ClassFunctionVisbilityCombobox->Visible = false;
		m_ClassFunctionNameInput->Visible = false;
		m_EventReturnStatusLabel->Visible = false;
		m_EventReturnStatusCombobox->Visible = false;

		m_GenerateClassFunctionCheckbox->Checked = false;
		m_ClassFunctionNameInput->Text = "";
		m_ClassFunctionVisbilityCombobox->SelectItem("private");
		m_EventReturnStatusCombobox->SelectItem("Handled");
	}
}
