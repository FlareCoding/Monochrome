#include "Studio.h"

void MonochromeStudio::Start()
{
	m_StudioWindow->StartWindowLoop();
}

void MonochromeStudio::Initialize()
{
	CreateStudioWindow();
	InitStudioUI();
}

void MonochromeStudio::CreateStudioWindow()
{
	m_StudioWindow = UIWindow::Create(WindowStyle::Modern, 1620, 980, "Monochrome Studio");
	m_StudioWindow->SetBackgroundColor(Color(41, 41, 42));
	m_StudioWindow->SetModernWindowButtonsColor(Color(41, 41, 42));
}

void MonochromeStudio::InitStudioUI()
{
#pragma region Dock Panel

	m_DockPanel = MakeRef<UIDockPanel>();
	m_DockPanel->layer.color = Color::transparent;
	m_DockPanel->SetAnchoredToWindow(true);
	m_StudioWindow->AddView(m_DockPanel);

#pragma endregion

#pragma region Left Docked Panel

	auto LeftDockedPanel = MakeRef<UIDockPanel>();
	LeftDockedPanel->anchor = Anchor::Left;
	LeftDockedPanel->layer.frame.size.width = 300;
	LeftDockedPanel->layer.color = Color::transparent;
	m_DockPanel->AddSubview(LeftDockedPanel);

#pragma endregion

#pragma region Right Docked Panel

	auto RightDockedPanel = MakeRef<UIDockPanel>();
	RightDockedPanel->anchor = Anchor::Right;
	RightDockedPanel->layer.frame.size.width = 300;
	RightDockedPanel->layer.color = Color::transparent;
	m_DockPanel->AddSubview(RightDockedPanel);

#pragma endregion

#pragma region Middle Docked Panel

	auto MiddleDockedPanel = MakeRef<UIDockPanel>();
	MiddleDockedPanel->anchor = Anchor::Center;
	MiddleDockedPanel->layer.color = Color::transparent;
	m_DockPanel->AddSubview(MiddleDockedPanel);

#pragma endregion

#pragma region Properties

	auto PropertiesLabel = MakeRef<UILabel>(Frame(0, 0, 0, 60));
	PropertiesLabel->anchor = Anchor::Top;
	PropertiesLabel->Text = "Properties";
	PropertiesLabel->Properties.FontSize = 18;
	PropertiesLabel->color = Color::white;
	LeftDockedPanel->AddSubview(PropertiesLabel);

	m_PropertiesPanel = MakeRef<UIScrollPanel>();
	m_PropertiesPanel->ScrollbarColor = Color(120, 120, 120);
	m_PropertiesPanel->layer.color = Color(51, 51, 51);
	m_PropertiesPanel->anchor = Anchor::Center;
	m_PropertiesPanel->ContentView->AddEventHandler<EventType::DockingUpdate>([this](Event&, UIView*) -> bool {
		m_PropertiesPanel->layer.frame.size.height = (float)m_StudioWindow->GetHeight() - 120;
		m_PropertiesPanel->ContentView->layer.frame.size.width = m_Toolbox->layer.frame.size.width;
		return EVENT_UNHANDLED;
	});
	m_PropertiesPanel->ContentView->layer.color = Color(51, 51, 52);
	LeftDockedPanel->AddSubview(m_PropertiesPanel);

#pragma endregion

#pragma region Workspace Area Tab View

	m_WorkspaceTabView = MakeRef<UITabView>(Frame(0, 0, 1000, 916));
	m_WorkspaceTabView->anchor = Anchor::Center;
	m_WorkspaceTabView->UnderlineTabs = true;
	m_WorkspaceTabView->AddTab("Editor")->layer.color = m_StudioWindow->GetBackgroundColor();
	m_WorkspaceTabView->AddTab("Settings")->layer.color = m_StudioWindow->GetBackgroundColor();
	m_WorkspaceTabView->OpenTab("Editor");
	m_WorkspaceTabView->SelectedTabColor = Color(61, 61, 62);
	m_WorkspaceTabView->StyleTabButtons([this](Ref<UIButton>& tab) {
		tab->layer.color = m_StudioWindow->GetBackgroundColor();
	});
	MiddleDockedPanel->AddSubview(m_WorkspaceTabView);

#pragma endregion

#pragma region Editor Tab

	auto EditorTab = MakeRef<UIDockPanel>(Frame(0, 0, 0, 0));
	EditorTab->layer.color = Color::transparent;
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(EditorTab);

	m_WorkspaceTabView->AddEventHandler<EventType::DockingUpdate>([this, EditorTab, MiddleDockedPanel](Event& e, UIView* sender) -> bool {
		if (reinterpret_cast<DockingUpdateEvent&>(e).SourcePanel == MiddleDockedPanel.get())
			EditorTab->layer.frame.size = m_WorkspaceTabView->layer.frame.size;

		return EVENT_UNHANDLED;
	});

#pragma endregion

#pragma region Project View Settings Bar

	m_ProjectSettingsBar = MakeRef<UIView>(Frame(0, 0, 0, 50));
	m_ProjectSettingsBar->anchor = Anchor::Top;
	m_ProjectSettingsBar->layer.color = Color::transparent;
	EditorTab->AddSubview(m_ProjectSettingsBar);

	auto ProjectWidthLabel = MakeRef<UILabel>(Frame(0, 0, 100, 50));
	ProjectWidthLabel->color = Color::white;
	ProjectWidthLabel->Text = "Window Width: ";
	ProjectWidthLabel->Properties.FontSize = 12;
	m_ProjectSettingsBar->AddSubview(ProjectWidthLabel);

	m_ProjectWidthTextbox = MakeRef<UITextbox>(Frame(100, 15, 100, 20));
	m_ProjectWidthTextbox->Placeholder = "Enter width";
	m_ProjectWidthTextbox->Text = "900";
	m_ProjectWidthTextbox->textProperties.FontSize = 14;
	m_ProjectWidthTextbox->CornerRadius = 2;
	m_ProjectWidthTextbox->layer.color = Color(58, 58, 59);
	m_ProjectWidthTextbox->TextColor = Color::white;
	m_ProjectWidthTextbox->FocusedHighlightColor = Color(28, 28, 29);
	m_ProjectWidthTextbox->SetZIndex(1);
	m_ProjectWidthTextbox->AddEventHandler<EventType::FocusChanged>([this](Event& e, UIView* sender) {
		if (!reinterpret_cast<FocusChangedEvent&>(e).GainedFocus)
		{
			try {
				float width = std::stof(reinterpret_cast<UITextbox*>(sender)->Text);
				m_ProjectView->SetProjectWidth(width);
			}
			catch (...) {}
		}

		return EVENT_HANDLED;
	});
	m_ProjectWidthTextbox->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_ENTER)
		{
			try {
				float width = std::stof(reinterpret_cast<UITextbox*>(sender)->Text);
				m_ProjectView->SetProjectWidth(width);
			}
			catch (...) {}
		}

		return EVENT_HANDLED;
	});
	m_ProjectSettingsBar->AddSubview(m_ProjectWidthTextbox);

	auto ProjectHeightLabel = MakeRef<UILabel>(Frame(210, 0, 100, 50));
	ProjectHeightLabel->color = Color::white;
	ProjectHeightLabel->Text = "Window Height: ";
	ProjectHeightLabel->Properties.FontSize = 12;
	m_ProjectSettingsBar->AddSubview(ProjectHeightLabel);

	m_ProjectHeightTextbox = MakeRef<UITextbox>(Frame(310, 15, 100, 20));
	m_ProjectHeightTextbox->Placeholder = "Enter Height";
	m_ProjectHeightTextbox->Text = "800";
	m_ProjectHeightTextbox->textProperties.FontSize = 14;
	m_ProjectHeightTextbox->CornerRadius = 2;
	m_ProjectHeightTextbox->layer.color = Color(58, 58, 59);
	m_ProjectHeightTextbox->TextColor = Color::white;
	m_ProjectHeightTextbox->FocusedHighlightColor = Color(28, 28, 29);
	m_ProjectHeightTextbox->SetZIndex(1);
	m_ProjectHeightTextbox->AddEventHandler<EventType::FocusChanged>([this](Event& e, UIView* sender) {
		if (!reinterpret_cast<FocusChangedEvent&>(e).GainedFocus)
		{
			try {
				float height = std::stof(reinterpret_cast<UITextbox*>(sender)->Text);
				m_ProjectView->SetProjectHeight(height);
			}
			catch (...) {}
		}

		return EVENT_HANDLED;
	});
	m_ProjectHeightTextbox->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_ENTER)
		{
			try {
				float height = std::stof(reinterpret_cast<UITextbox*>(sender)->Text);
				m_ProjectView->SetProjectHeight(height);
			}
			catch (...) {}
		}

		return EVENT_HANDLED;
	});
	m_ProjectSettingsBar->AddSubview(m_ProjectHeightTextbox);

	auto ProjectColorLabel = MakeRef<UILabel>(Frame(420, 0, 100, 50));
	ProjectColorLabel->color = Color::white;
	ProjectColorLabel->Text = "Window Color: ";
	ProjectColorLabel->Properties.FontSize = 12;
	m_ProjectSettingsBar->AddSubview(ProjectColorLabel);

	auto ProjectColorTextbox = MakeRef<UITextbox>(Frame(520, 15, 120, 20));
	ProjectColorTextbox->Placeholder = "Enter Color";
	ProjectColorTextbox->Text = "170, 170, 170";
	ProjectColorTextbox->textProperties.FontSize = 14;
	ProjectColorTextbox->CornerRadius = 2;
	ProjectColorTextbox->layer.color = Color(58, 58, 59);
	ProjectColorTextbox->TextColor = Color::white;
	ProjectColorTextbox->FocusedHighlightColor = Color(28, 28, 29);
	ProjectColorTextbox->SetZIndex(1);
	ProjectColorTextbox->AddEventHandler<EventType::FocusChanged>([this](Event& e, UIView* sender) {
		if (!reinterpret_cast<FocusChangedEvent&>(e).GainedFocus)
			m_ProjectView->layer.color = Color::FromRGBString(reinterpret_cast<UITextbox*>(sender)->Text.c_str());

		return EVENT_HANDLED;
	});
	ProjectColorTextbox->AddEventHandler<EventType::KeyPressed>([this](Event& e, UIView* sender) {
		if (reinterpret_cast<KeyPressedEvent&>(e).keycode == KeyCode::KEY_ENTER)
			m_ProjectView->layer.color = Color::FromRGBString(reinterpret_cast<UITextbox*>(sender)->Text.c_str());

		return EVENT_HANDLED;
	});
	m_ProjectSettingsBar->AddSubview(ProjectColorTextbox);

#pragma endregion

#pragma region Project View Resizing Label

	m_ProjectViewResizeLabel = MakeRef<UILabel>(Frame(0, 0, 40, 40));
	m_ProjectViewResizeLabel->SetZIndex(30);
	m_ProjectViewResizeLabel->cursor = CursorType::SizeNWE;
	m_ProjectViewResizeLabel->color = Color::white;
	m_ProjectViewResizeLabel->UseWidestringText = true;
	m_ProjectViewResizeLabel->WidestringText = L"■";

	m_ProjectViewResizeLabel->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = false;
		m_ViewClickedLocationOffset = sender->srcwindow->GetMouseCursorPos() - sender->GetAbsolutePosition();
		
		std::thread dragging_thread(&MonochromeStudio::DragResizeLabel, this);
		dragging_thread.detach();

		return EVENT_HANDLED;
	});
	m_ProjectViewResizeLabel->AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = true;
		return EVENT_HANDLED;
	});
	m_ProjectViewResizeLabel->AddEventHandler<EventType::FocusChanged>([this](Event& e, UIView* sender) {
		m_MouseButtonReleasedFromView = true;
		return EVENT_HANDLED;
	});

	EditorTab->AddSubview(m_ProjectViewResizeLabel);

#pragma endregion

#pragma region Project View

	m_ProjectView = MakeRef<ProjectView>(m_PropertiesPanel, m_ProjectViewResizeLabel);
	EditorTab->AddSubview(m_ProjectView);
	m_ProjectView->Initialize(); // must be called after being added to the parent view

#pragma endregion

#pragma region Toolbox

	auto ToolboxLabel = MakeRef<UILabel>(Frame(0, 0, 0, 60));
	ToolboxLabel->anchor = Anchor::Top;
	ToolboxLabel->Text = "Widgets";
	ToolboxLabel->Properties.FontSize = 18;
	ToolboxLabel->color = Color::white;
	RightDockedPanel->AddSubview(ToolboxLabel);

	m_Toolbox = MakeRef<UIScrollPanel>();
	m_Toolbox->ContentView->layer.frame.size.height = 1400;
	m_Toolbox->ScrollbarColor = Color(120, 120, 120);
	m_Toolbox->layer.color = Color(51, 51, 52);
	m_Toolbox->anchor = Anchor::Center;
	m_Toolbox->ContentView->AddEventHandler<EventType::DockingUpdate>([this](Event&, UIView*) -> bool {
		m_Toolbox->layer.frame.size.height = (float)m_StudioWindow->GetHeight() - 120;
		m_Toolbox->ContentView->layer.frame.size.width = m_Toolbox->layer.frame.size.width;
		return EVENT_UNHANDLED;
	});
	m_Toolbox->ContentView->layer.color = Color(51, 51, 52);
	RightDockedPanel->AddSubview(m_Toolbox);

	const auto MakeToolboxWidgetButton = [this](const std::string& name, WidgetType type) {
		static Size WidgetButtonSize = Size{ 296.0f, 60.0f };
		static float PreviousPosY = 2.0f;

		Ref<UIButton> ToolboxWidgetButton = MakeRef<UIButton>();
		ToolboxWidgetButton->layer.frame = Frame(Position{ 2, PreviousPosY }, WidgetButtonSize);
		ToolboxWidgetButton->Label->Text = name;
		ToolboxWidgetButton->Label->color = Color::white;
		ToolboxWidgetButton->Label->Properties.FontSize = 15;
		ToolboxWidgetButton->layer.color = Color(61, 61, 62);
		ToolboxWidgetButton->CornerRadius = 0;

		ToolboxWidgetButton->AddEventHandler<EventType::MouseButtonClicked>([this, type](Event& e, UIView* sender) -> bool {
			m_ProjectView->AddWidget(type);
			return EVENT_HANDLED;
		});

		m_Toolbox->AddChild(ToolboxWidgetButton);
		PreviousPosY += 62.0f;

		m_Toolbox->ContentView->layer.frame.size.height += 62.0f;
	};

	m_Toolbox->ContentView->layer.frame.size.height = 2;

	MakeToolboxWidgetButton("View",				WidgetType::View);
	MakeToolboxWidgetButton("Label",			WidgetType::Label);
	MakeToolboxWidgetButton("Button",			WidgetType::Button);
	MakeToolboxWidgetButton("Checkbox",			WidgetType::Checkbox);
	MakeToolboxWidgetButton("Slider",			WidgetType::Slider);
	MakeToolboxWidgetButton("Textbox",			WidgetType::Textbox);
	MakeToolboxWidgetButton("Combobox",			WidgetType::Combobox);
	MakeToolboxWidgetButton("Text Area",		WidgetType::TextArea);
	MakeToolboxWidgetButton("Image",			WidgetType::Image);
	MakeToolboxWidgetButton("Scroll Panel",		WidgetType::ScrollPanel);
	MakeToolboxWidgetButton("Docking Panel",	WidgetType::DockingPanel);
	MakeToolboxWidgetButton("Tab View",			WidgetType::TabView);

#pragma endregion

}

void MonochromeStudio::DragResizeLabel()
{
	Position PreviousMousePosition = 
		m_ProjectView->srcwindow ? m_ProjectView->srcwindow->GetMouseCursorPos() : Position{ 0, 0 };

	while (!m_MouseButtonReleasedFromView)
	{
		Point current_pos = m_ProjectViewResizeLabel->layer.frame.position;
		auto cursor_pos = m_ProjectViewResizeLabel->srcwindow->GetMouseCursorPos();

		Size MouseMovedDistance = cursor_pos - PreviousMousePosition;
		PreviousMousePosition = cursor_pos;

		float new_width = m_ProjectView->layer.frame.size.width + MouseMovedDistance.x;
		float new_height = m_ProjectView->layer.frame.size.height + MouseMovedDistance.y;

		m_ProjectView->SetProjectWidth(new_width);
		m_ProjectView->SetProjectHeight(new_height);

		m_ProjectWidthTextbox->Text = std::to_string((uint32_t)new_width);
		m_ProjectHeightTextbox->Text = std::to_string((uint32_t)new_height);

		Sleep(1);
	}
}
