#include "ProjectView.h"

#define CREATE_WIDGET(widget_t, width, height) widget = MakeRef<widget_t>( \
										Frame(placement_position.x, placement_position.y, width, height) \
									);

ProjectView::ProjectView(Ref<UIScrollPanel> PropertiesPanel)
	: UIScrollPanel(Frame(0, 50, 0, 0))
{
	m_PropertiesPanel = PropertiesPanel;
	WidgetProperties::CreatePropertiesList();

	layer.color = Color(170, 170, 170);
	ContentView->layer.frame.size = Size{ m_ProjectWidth, m_ProjectHeight };
	CornerRadius = 2;

	SetZIndex(2);
}

void ProjectView::Initialize()
{
	SetupEventHandlers();

	m_BackgroundView = MakeRef<UIView>(Frame(0, 0, layer.frame.size.width, layer.frame.size.height));
	m_BackgroundView->layer.color = Color::transparent;
	m_BackgroundView->SetZIndex(0);
	m_BackgroundView->AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) {
		m_PropertiesPanel->Clear();
		return EVENT_UNHANDLED;
	});
	AddSubview(m_BackgroundView);
}

void ProjectView::SetupEventHandlers()
{
	AddEventHandler<EventType::DockingUpdate>([this](Event& e, UIView* sender) {
		UpdateSize();
		return EVENT_UNHANDLED;
	});

	AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) {
		return EVENT_UNHANDLED;
	});
}

void ProjectView::UpdateSize()
{
	layer.frame.size = Size{ parent->layer.frame.size.width, parent->layer.frame.size.height - 80 };
	ContentView->layer.frame.size = Size{ m_ProjectWidth, m_ProjectHeight };

	if (layer.frame.size.width > m_ProjectWidth || layer.frame.size.height > m_ProjectHeight)
	{
		layer.frame.size = Size{ m_ProjectWidth, m_ProjectHeight };
	}

	m_BackgroundView->layer.frame.size = layer.frame.size;
}

void ProjectView::SetProjectWidth(float width)
{
	m_ProjectWidth = width;
	UpdateSize();
}

void ProjectView::SetProjectHeight(float height)
{
	m_ProjectHeight = height;
	UpdateSize();
}

void ProjectView::AddWidget(WidgetType type)
{
	static Position placement_position = { 180, 180 };
	static uint32_t zIndex = 0;
	zIndex++;
	
	Ref<UIView> widget = nullptr;

	switch (type)
	{
	case WidgetType::View: { CREATE_WIDGET(UIView, 380, 320); break; }
	case WidgetType::Label: { CREATE_WIDGET(UILabel, 180, 60); break; }
	case WidgetType::Button: { CREATE_WIDGET(UIButton, 180, 60); break; }
	case WidgetType::Checkbox: { CREATE_WIDGET(UICheckbox, 180, 40); break; }
	case WidgetType::Slider: { CREATE_WIDGET(UISlider, 200, 20); reinterpret_cast<UISlider*>(widget.get())->Value = 60.0f;  break; }
	case WidgetType::Textbox: { CREATE_WIDGET(UITextbox, 200, 30); break; }
	default: break;
	}

	widget->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = false;
		m_TargetView = sender;
		m_ViewClickedLocationOffset = sender->srcwindow->GetMouseCursorPos() - sender->GetAbsolutePosition();

		std::thread dragging_thread(&ProjectView::DragView, this);
		dragging_thread.detach();

		return EVENT_HANDLED;
	});

	widget->AddEventHandler<EventType::MouseButtonReleased>([this, type](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = true;
		OpenWidgetProperties(type);

		return EVENT_HANDLED;
	});

	widget->AddEventHandler<EventType::FocusChanged>([](Event&, UIView*) { return EVENT_UNHANDLED; });

	widget->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_BACKSPACE)
		{
			ContentView->RemoveSubview(sender);
			m_PropertiesPanel->Clear();
		}

		return EVENT_HANDLED;
	});

	widget->SetZIndex(zIndex);
	
	AddChild(widget);
}

void ProjectView::OpenWidgetProperties(WidgetType widget_type)
{
	// Clear any previous properties
	m_PropertiesPanel->Clear();
	
	float y_offset = 4;

	for (auto& property : WidgetProperties::WidgetPropertiesList[widget_type])
	{
		switch (property.type)
		{
		case WidgetProperties::Flags::TOGGLABLE:
		{
			y_offset += AddTogglableProperty(property, y_offset);
			break;
		}
		case WidgetProperties::Flags::DATA_ENTRY:
		{
			y_offset += AddDataEntryProperty(property, y_offset);
			break;
		}
		case WidgetProperties::Flags::SELECTABLE:
		{
			y_offset += AddSelectableProperty(property, y_offset);
			break;
		}
		default: break;
		}
	}

	m_PropertiesPanel->ContentView->layer.frame.size.height = y_offset;

	// If the last element is a combobox, extend
	// the content view to fit in all the elements.
	if (m_PropertiesPanel->ContentView->subviews.size())
	{
		auto element = m_PropertiesPanel->ContentView->subviews.at(
			m_PropertiesPanel->ContentView->subviews.size() - 1
		);

		if (StudioUtils::CheckType<UICombobox>(element.get()))
		{
			auto cbox = std::dynamic_pointer_cast<UICombobox>(element);
			m_PropertiesPanel->ContentView->layer.frame.size.height += cbox->GetItemCount() * cbox->SlotSize + 10.0f;
		}
	}
}

float ProjectView::AddTogglableProperty(WidgetProperties::Property prop, float y_offset)
{
	auto label = MakeRef<UILabel>(Frame(20, y_offset, 120, 30));
	label->color = Color::white;
	label->Text = prop.name;
	label->Properties.FontSize = 15;
	label->Properties.Alignment = TextAlignment::LEADING;

	auto false_button = MakeRef<UIButton>(Frame(150, y_offset + 5, 60, 20));
	false_button->CornerRadius = 0;
	false_button->layer.color = prop.getter_fn(m_TargetView) == "0" ? Color(97, 39, 46) : Color(109, 113, 109);
	false_button->Label->color = Color::white;
	false_button->Label->Text = "false";

	auto true_button = MakeRef<UIButton>(Frame(210, y_offset + 5, 60, 20));
	true_button->CornerRadius = 0;
	true_button->layer.color = prop.getter_fn(m_TargetView) == "1" ? Color(39, 97, 46) : Color(109, 113, 109);
	true_button->Label->color = Color::white;
	true_button->Label->Text = "true";
	
	UIButton* false_button_raw_ptr = false_button.get();
	UIButton* true_button_raw_ptr = true_button.get();

	false_button->AddEventHandler<EventType::MouseButtonClicked>([this, false_button_raw_ptr, true_button_raw_ptr, prop](Event& e, UIView* sender) {
		false_button_raw_ptr->layer.color = Color(97, 39, 46);
		true_button_raw_ptr->layer.color = Color(109, 113, 109);

		prop.applier_fn(m_TargetView, "0");
		return EVENT_HANDLED;
	});

	true_button->AddEventHandler<EventType::MouseButtonClicked>([this, false_button_raw_ptr, true_button_raw_ptr, prop](Event& e, UIView* sender) {
		false_button_raw_ptr->layer.color = Color(109, 113, 109);
		true_button_raw_ptr->layer.color = Color(39, 97, 46);

		prop.applier_fn(m_TargetView, "1");
		return EVENT_HANDLED;
	});

	m_PropertiesPanel->AddChild(label);
	m_PropertiesPanel->AddChild(false_button);
	m_PropertiesPanel->AddChild(true_button);

	return 30.0f;
}

float ProjectView::AddDataEntryProperty(WidgetProperties::Property prop, float y_offset)
{
	auto label = MakeRef<UILabel>(Frame(20, y_offset, 120, 30));
	label->color = Color::white;
	label->Text = prop.name;
	label->Properties.FontSize = 15;
	label->Properties.Alignment = TextAlignment::LEADING;

	auto textbox = MakeRef<UITextbox>(Frame(150, y_offset + 5, 120, 20));
	textbox->Placeholder = "enter value";
	textbox->Text = prop.getter_fn(m_TargetView);
	textbox->textProperties.FontSize = 14;
	textbox->CornerRadius = 2;
	textbox->layer.color = Color(58, 58, 59, 1.0f);
	textbox->TextColor = Color::white;
	textbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);

	UITextbox* textbox_raw_ptr = textbox.get();

	textbox->AddEventHandler<EventType::FocusChanged>([textbox_raw_ptr, prop, this](Event& e, UIView* sender) {
		if (!reinterpret_cast<FocusChangedEvent&>(e).GainedFocus)
			prop.applier_fn(m_TargetView, textbox_raw_ptr->Text);

		return EVENT_HANDLED; 
	});
	textbox->AddEventHandler<EventType::KeyPressed>([textbox_raw_ptr, prop, this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_ENTER)
			prop.applier_fn(m_TargetView, textbox_raw_ptr->Text);

		return EVENT_HANDLED;
	});

	m_PropertiesPanel->AddChild(label);
	m_PropertiesPanel->AddChild(textbox);

	return 30.0f;
}

float ProjectView::AddSelectableProperty(WidgetProperties::Property prop, float y_offset)
{
	auto label = MakeRef<UILabel>(Frame(20, y_offset, 120, 30));
	label->color = Color::white;
	label->Text = prop.name;
	label->Properties.FontSize = 15;
	label->Properties.Alignment = TextAlignment::LEADING;

	auto combobox = MakeRef<UICombobox>(Frame(150, y_offset + 5, 120, 20));
	combobox->SetItems(prop.items);
	combobox->SelectItem(std::stoi(prop.getter_fn(m_TargetView)));
	combobox->SetItemBackgroundColor(Color(58, 58, 59, 1));
	combobox->SetItemTextColor(Color::white);
	combobox->layer.color = Color(62, 62, 63, 1);
	combobox->SetDropdownArrowColor(Color::white);
	combobox->SetSelectedItemColor(Color::white);
	combobox->AddItemChangedEventHandler([this, prop](size_t index, UICombobox* sender) {
		prop.applier_fn(m_TargetView, std::to_string(sender->IndexOf(sender->GetSelectedItem())));
	});

	m_PropertiesPanel->AddChild(label);
	m_PropertiesPanel->AddChild(combobox);

	return 30.0f;
}

void ProjectView::DragView()
{
	Position PreviousMousePosition =
		m_TargetView->srcwindow ? m_TargetView->srcwindow->GetMouseCursorPos() : Position{ 0, 0 };

	while (!m_MouseButtonReleasedFromView)
	{
		if (m_TargetView->srcwindow)
		{
			Point current_pos = m_TargetView->layer.frame.position;
			auto cursor_pos = m_TargetView->srcwindow->GetMouseCursorPos();

			Size MouseMovedDistance = cursor_pos - PreviousMousePosition;
			PreviousMousePosition = cursor_pos;

			m_TargetView->layer.frame.position += MouseMovedDistance;

			if (StudioSettings::GridSnappingEnabled)
			{
				// TO-DO
			}

			if (m_TargetView->layer.frame.position.x < (-1.0f * m_TargetView->layer.frame.size.width / 2) ||
				m_TargetView->layer.frame.position.y < (-1.0f * m_TargetView->layer.frame.size.height / 2))
			{
				m_TargetView->layer.frame.position = current_pos;
			}
		}

		Sleep(2);
	}
}
