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
	case WidgetType::View:			{ CREATE_WIDGET(UIView, 380, 320); break; }
	case WidgetType::Label:			{ CREATE_WIDGET(UILabel, 180, 60); break; }
	case WidgetType::Button:		{ CREATE_WIDGET(UIButton, 180, 60); break; }
	case WidgetType::Checkbox:		{ CREATE_WIDGET(UICheckbox, 180, 40); break; }
	case WidgetType::Slider:		{ CREATE_WIDGET(UISlider, 200, 20); reinterpret_cast<UISlider*>(widget.get())->Value = 60.0f;  break; }
	case WidgetType::Textbox:		{ CREATE_WIDGET(UITextbox, 200, 30); break; }
	case WidgetType::Combobox:		{ CREATE_WIDGET(UICombobox, 200, 30); break; }
	case WidgetType::TextArea:		{ CREATE_WIDGET(UITextArea, 280, 200); reinterpret_cast<UITextArea*>(widget.get())->Text = "Sample Text"; break; }
	case WidgetType::Image:			{ CREATE_WIDGET(UIImage, 250, 250); break; }
	case WidgetType::ScrollPanel:	{ CREATE_WIDGET(UIScrollPanel, 350, 400); break; }
	case WidgetType::DockingPanel:	{ CREATE_WIDGET(UIDockPanel, 350, 350); break; }
	case WidgetType::TabView:		{ CREATE_WIDGET(UITabView, 450, 350); reinterpret_cast<UITabView*>(widget.get())->AddTab("Tab 1");  break; }
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
		
		CheckEmbeddingNecessity();

		return EVENT_HANDLED;
	});

	widget->AddEventHandler<EventType::WindowClosed>([this](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = true;
		return EVENT_UNHANDLED;
	});

	widget->AddEventHandler<EventType::FocusChanged>([](Event&, UIView*) { return EVENT_UNHANDLED; });

	widget->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_BACKSPACE)
		{
			if (srcwindow)
			{
				for (auto& view : ContentView->subviews)
				{
					if (view.get() == sender)
					{
						RemoveChild(view);
						break;
					}
				}

				m_PropertiesPanel->Clear();
				srcwindow->FocusView(nullptr);
			}
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

void ProjectView::CheckEmbeddingNecessity()
{
	/*Ref<UIView> TargetViewRef = nullptr;
	for (auto& view : m_TargetView->parent->subviews)
	{
		if (view.get() == m_TargetView)
		{
			TargetViewRef = view;
			break;
		}
	}

	bool should_unembed = true;
	while (should_unembed)
		should_unembed = UnembedViewIfNeeded(TargetViewRef);

	Ref<UIView> embeddable = nullptr;
	auto* elements = &ContentView->subviews;

	while (CheckIfViewNeedsEmbedding(TargetViewRef, *elements, embeddable))
		elements = &embeddable->subviews;

	if (embeddable && embeddable.get() != m_TargetView->parent)
	{
		EmbedView(TargetViewRef, embeddable);

		if (m_TargetView->parent)
			m_TargetView->parent->RemoveSubview(TargetViewRef);

		srcwindow->FocusView(nullptr);
	}*/

	Ref<UIView> TargetViewRef = nullptr;
	for (auto& view : m_TargetView->parent->subviews)
	{
		if (view.get() == m_TargetView)
		{
			TargetViewRef = view;
			break;
		}
	}

	for (auto view : ContentView->subviews)
	{
		if (view.get() == m_TargetView) continue;

		Frame frame = view->layer.frame;
		frame.position = view->GetAbsolutePosition();
		
		if (frame.DoesContain(srcwindow->GetMouseCursorPos()))
		{
			//UIView* parent_view = view->parent;
			//view->layer.frame.position += parent_view->layer.frame.position;
			if (StudioUtils::IsViewEmbeddable(view.get()))
			{
				m_TargetView->parent->RemoveSubview(TargetViewRef);
				view->AddSubview(TargetViewRef);

				auto position_offset = srcwindow->GetMouseCursorPos() - view->GetAbsolutePosition();
				m_TargetView->layer.frame.position = position_offset - m_ViewClickedLocationOffset;

				srcwindow->FocusView(nullptr);
			}
		}
	}
}

//bool ProjectView::UnembedViewIfNeeded(Ref<UIView> view)
//{
//	bool result = false;
//
//	Ref<UIView> TargetViewRef = nullptr;
//	for (auto& view : view->parent->subviews)
//	{
//		if (view.get() == m_TargetView)
//		{
//			TargetViewRef = view;
//			break;
//		}
//	}
//
//	if (view->parent == this->ContentView.get())
//		return false;
//
//	// If the view inside another view, check if it needs to be taken out of it.
//	if (view->parent)
//	{
//		Frame frame = view->parent->layer.frame;
//		frame.position = view->parent->GetAbsolutePosition();
//
//		if (!frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
//		{
//			UIView* parent_view = view->parent;
//
//			view->layer.frame.position += parent_view->layer.frame.position;
//			parent_view->RemoveSubview(TargetViewRef);
//
//			if (parent_view->parent)
//				parent_view->parent->AddSubview(view);
//
//			result = true;
//		}
//	}
//
//	return result;
//}
//
//bool ProjectView::CheckIfViewNeedsEmbedding(Ref<UIView>& view_being_checked, std::vector<Ref<UIView>>& elements, Ref<UIView>& embeddable)
//{
//	if (!view_being_checked->srcwindow)
//		return false;
//
//	for (auto& view : elements)
//	{
//		if (!view->srcwindow)
//			continue;
//
//		if (view.get() == view_being_checked.get()) continue;
//
//		Frame frame = view->layer.frame;
//		frame.position = view->GetAbsolutePosition();
//
//		if (frame.DoesContain(view->srcwindow->GetMouseCursorPos()))
//		{
//			embeddable = view;
//			return true;
//		}
//	}
//
//	return false;
//}
//
//void ProjectView::EmbedView(Ref<UIView> src, Ref<UIView> dest)
//{
//	auto it = std::find(src->parent->subviews.begin(), src->parent->subviews.end(), src);
//	if (it != src->parent->subviews.end())
//	{
//		src->parent->subviews.erase(it);
//		src->parent->RemoveSubview(src);
//	}
//
//	auto position_offset = dest->srcwindow->GetMouseCursorPos() - dest->GetAbsolutePosition();
//	src->layer.frame.position = position_offset - m_ViewClickedLocationOffset;
//
//	dest->AddSubview(src);
//}
