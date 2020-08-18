#include "Editor.h"
#include <ShlObj.h>
#include <filesystem>
#include <fstream>

void MonochromeEditor::Start()
{
	m_EditorWindow->StartWindowLoop();
}

void MonochromeEditor::Initialize()
{
	CreateEditorWindow();
	InitEditorUI();

	m_LabelProperties.Initialize(m_PropertiesView, &m_ProjectWindow);
	m_ButtonProperties.Initialize(m_PropertiesView, &m_ProjectWindow);
	m_PlainViewProperties.Initialize(m_PropertiesView, &m_ProjectWindow);
	m_VariableCodeProperties.Initialize(m_PropertiesView, &m_ProjectWindow);
	m_PWScriptController.Initialize(&m_ProjectWindow, &m_ProjectUIElements);

	m_PropertiesView->subviews.clear();

	LoadConfiguration();
}

void MonochromeEditor::SaveConfiguration()
{
	char appdata_path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, NULL, appdata_path)))
	{
		std::string config_path = std::string(appdata_path) + "\\MCEditor\\settings.cfg";

		std::ofstream cfg(config_path);
		cfg << "[Monochrome Paths]" << std::endl;
		cfg << "MCsrc: " << m_MonochromeSourcePathTextbox->Text << std::endl;
		cfg << "MClibdbg: " << m_MonochromeLibDbgPathTextbox->Text << std::endl;
		cfg << "MClibrel: " << m_MonochromeLibRelPathTextbox->Text << std::endl;
		cfg.close();
	}
}

void MonochromeEditor::LoadConfiguration()
{
	char appdata_path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, NULL, appdata_path)))
	{
		if (!std::filesystem::is_directory(std::string(appdata_path) + "\\MCEditor"))
			std::filesystem::create_directory(std::string(appdata_path) + "\\MCEditor");

		std::string config_path = std::string(appdata_path) + "\\MCEditor\\settings.cfg";
		if (std::filesystem::exists(config_path))
		{
			std::string line;
			std::ifstream cfg(config_path);
			while (std::getline(cfg, line))
			{
				if (line.find("MCsrc: ") != std::string::npos) m_MonochromeSourcePathTextbox->Text = line.substr(7, line.size() - 7);
				if (line.find("MClibdbg: ") != std::string::npos) m_MonochromeLibDbgPathTextbox->Text = line.substr(10, line.size() - 10);
				if (line.find("MClibrel: ") != std::string::npos) m_MonochromeLibRelPathTextbox->Text = line.substr(10, line.size() - 10);
			}
		}
	}
}

void MonochromeEditor::CreateEditorWindow()
{
	m_EditorWindow = UIWindow::Create(WindowStyle::Modern, m_EditorWidth, m_EditorHeight, "Monochrome Editor");
	m_EditorWindow->SetBackgroundColor(Color(41, 41, 42, 1.0f));
	m_EditorWindow->SetModernWindowButtonsColor(Color(41, 41, 42, 1.0f));
}

void MonochromeEditor::InitEditorUI()
{
#pragma region Toolbox

	auto ToolboxLabel = MakeRef<UILabel>(Frame(Position{ (float)m_EditorWidth - 300, 60 }, Size{ 300, 40 }));
	ToolboxLabel->Text = "Widgets";
	ToolboxLabel->Properties.FontSize = 18;
	ToolboxLabel->color = Color::white;
	m_EditorWindow->AddView(ToolboxLabel);

	m_Toolbox = MakeRef<UIScrollPanel>();
	m_Toolbox->layer.frame = Frame(Position{ (float)m_EditorWidth - 300, 120 }, Size{ 300, (float)m_EditorHeight - 120 });
	m_Toolbox->layer.color = Color::transparent;
	m_Toolbox->ContentView->layer.frame.size.height = (float)m_EditorHeight - 120;
	m_Toolbox->ContentView->layer.color = Color(51, 51, 52, 1.0f);

	const auto MakeToolboxWidgetButton = [this](const std::string& name, std::function<void()> click_cb) -> Ref<UIButton> {
		static Size WidgetButtonSize = Size{ 296.0f, 60.0f };
		static float PreviousPosY = 2.0f;

		Ref<UIButton> ToolboxWidgetButton = MakeRef<UIButton>();
		ToolboxWidgetButton->layer.frame = Frame(Position{ 2, PreviousPosY }, WidgetButtonSize);
		ToolboxWidgetButton->Label->Text = name;
		ToolboxWidgetButton->Label->color = Color::white;
		ToolboxWidgetButton->Label->Properties.FontSize = 15;
		ToolboxWidgetButton->layer.color = Color(61, 61, 62, 1.0f);
		ToolboxWidgetButton->CornerRadius = 0;

		ToolboxWidgetButton->AddEventHandler<EventType::MouseButtonClicked>([this, click_cb](Event& e, UIView* sender) -> bool {
			if (m_ElementPreviewArea->subviews.size())
				m_VariableCodeProperties.UnregisterElement(m_ElementPreviewArea->subviews.at(0));

			m_OpenVariablePropertiesButton->Label->Text = "Variable Properties";
			m_ElementPreviewArea->subviews.clear();

			click_cb();
			return EVENT_HANDLED;
		});

		m_Toolbox->AddChild(ToolboxWidgetButton);
		PreviousPosY += 62.0f;

		return ToolboxWidgetButton;
	};

	static Position DefaultElementPreviewPosition = Position{ 360, 200 };

	auto ToolboxWidgetButton_UIView = MakeToolboxWidgetButton("UIView", [this]() {
		Ref<UIView> widget = MakeRef<UIView>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		widget->layer.frame.size = { 500, 400 };
		m_ElementPreviewArea->AddSubview(widget);
		m_SelectedTargetView = widget;

		OpenElementProperties(widget);
	});

	auto ToolboxWidgetButton_Label = MakeToolboxWidgetButton("Label", [this]() {
		Ref<UILabel> widget = MakeRef<UILabel>();
		widget->color = Color::white;
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);
		m_SelectedTargetView = widget;

		OpenElementProperties(widget);
	});

	auto ToolboxWidgetButton_Button	= MakeToolboxWidgetButton("Button", [this]() {
		Ref<UIButton> widget = MakeRef<UIButton>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);
		m_SelectedTargetView = widget;

		OpenElementProperties(widget);
	});

	auto ToolboxWidgetButton_Checkbox = MakeToolboxWidgetButton("Checkbox", [this]() {
		Ref<UICheckbox> widget = MakeRef<UICheckbox>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);
		m_SelectedTargetView = widget;

		OpenElementProperties(widget);
	});
	
	auto ToolboxWidgetButton_Slider	= MakeToolboxWidgetButton("Slider", [this]() {
		Ref<UISlider> widget = MakeRef<UISlider>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		widget->SliderKnobShape = Shape::Circle;
		widget->VisibleTickmarks = false;
		m_ElementPreviewArea->AddSubview(widget);
		m_SelectedTargetView = widget;

		OpenElementProperties(widget);
	});

	m_EditorWindow->AddView(m_Toolbox);

#pragma endregion

#pragma region Workspace

	m_WorkspaceTabView = MakeRef<UITabView>(Frame((float)m_EditorWidth / 2.0f - 500, 60, 1000, 916));
	m_WorkspaceTabView->UnderlineTabs = true;
	m_WorkspaceTabView->AddTab("Editor")->layer.color = m_EditorWindow->GetBackgroundColor();
	m_WorkspaceTabView->AddTab("Settings")->layer.color = m_EditorWindow->GetBackgroundColor();
	m_WorkspaceTabView->OpenTab("Editor");
	m_WorkspaceTabView->StyleTabButtons([this](Ref<UIButton>& tab) {
		tab->layer.color = m_EditorWindow->GetBackgroundColor();
	});
	m_WorkspaceTabView->SelectedTabColor = Color(61, 61, 62, 1.0f);
	m_EditorWindow->AddView(m_WorkspaceTabView);

#pragma endregion

#pragma region Element Preview Area

	m_ElementPreviewArea = MakeRef<UIView>();
	m_ElementPreviewArea->layer.frame = Frame(Position{ m_WorkspaceTabView->layer.frame.size.width / 2.0f - 448, m_WorkspaceTabView->layer.frame.size.height - 450 }, Size{ 896, 448 });
	m_ElementPreviewArea->layer.color = Color(51, 51, 52, 1.0f);
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(m_ElementPreviewArea);

	m_AddElementToProjectWindowButton = MakeRef<UIButton>();
	Position AddElemToProjWindowBtnPosition = Position{ m_ElementPreviewArea->layer.frame.size.width - 160, 2 };
	AddElemToProjWindowBtnPosition += m_ElementPreviewArea->layer.frame.position;
	m_AddElementToProjectWindowButton->layer.frame = Frame(AddElemToProjWindowBtnPosition, Size{ 158, 40 });
	m_AddElementToProjectWindowButton->Label->Text = "Add to Window";
	m_AddElementToProjectWindowButton->Label->color = Color::white;
	m_AddElementToProjectWindowButton->Label->Properties.FontSize = 15;
	m_AddElementToProjectWindowButton->layer.color = Color(61, 61, 62, 1.0f);
	m_AddElementToProjectWindowButton->CornerRadius = 4;
	m_AddElementToProjectWindowButton->SetZIndex(1000);
	m_AddElementToProjectWindowButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		AddElementToProjectWindow();
		return EVENT_HANDLED;
	});
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(m_AddElementToProjectWindowButton);

#pragma endregion

#pragma region Element Properties Area

	m_PropertiesView = MakeRef<UIView>();
	m_PropertiesView->layer.frame = Frame(Position{ 0, 0 }, Size{ 996, 440 });
	m_PropertiesView->layer.color = Color(51, 51, 52, 1.0f);
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(m_PropertiesView);

	m_OpenVariablePropertiesButton = MakeRef<UIButton>();
	Position ShowVariablePropertiesBtnPosition = m_PropertiesView->layer.frame.position + m_PropertiesView->layer.frame.size;
	ShowVariablePropertiesBtnPosition -= { 160, 42 };
	m_OpenVariablePropertiesButton->layer.frame = Frame(ShowVariablePropertiesBtnPosition, Size{ 158, 40 });
	m_OpenVariablePropertiesButton->Label->Text = "Variable Properties";
	m_OpenVariablePropertiesButton->Label->color = Color::white;
	m_OpenVariablePropertiesButton->Label->Properties.FontSize = 15;
	m_OpenVariablePropertiesButton->layer.color = Color(61, 61, 62, 1.0f);
	m_OpenVariablePropertiesButton->CornerRadius = 4;
	m_OpenVariablePropertiesButton->SetZIndex(1000);
	m_OpenVariablePropertiesButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		// Nothing should be done if no element is being previewed
		if (!m_SelectedTargetView) return EVENT_HANDLED;

		if (m_OpenVariablePropertiesButton->Label->Text._Equal("Variable Properties"))
		{
			// Open Variable Properties
			m_PropertiesView->subviews.clear();
			m_VariableCodeProperties.Open(m_SelectedTargetView);

			m_OpenVariablePropertiesButton->Label->Text = "Element Properties";
		}
		else
		{
			// Open Element Properties
			m_PropertiesView->subviews.clear();
			OpenElementProperties(m_SelectedTargetView);

			m_OpenVariablePropertiesButton->Label->Text = "Variable Properties";
		}

		return EVENT_HANDLED;
	});
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(m_OpenVariablePropertiesButton);

	m_DeleteElementButton = MakeRef<UIButton>();
	Position DeleteElementBtnPosition = m_PropertiesView->layer.frame.position + m_PropertiesView->layer.frame.size;
	DeleteElementBtnPosition -= { 322, 42 };
	m_DeleteElementButton->layer.frame = Frame(DeleteElementBtnPosition, Size{ 158, 40 });
	m_DeleteElementButton->Label->Text = "Delete Element";
	m_DeleteElementButton->Label->color = Color::white;
	m_DeleteElementButton->Label->Properties.FontSize = 15;
	m_DeleteElementButton->layer.color = Color(61, 61, 62, 1.0f);
	m_DeleteElementButton->CornerRadius = 4;
	m_DeleteElementButton->SetZIndex(1000);
	m_DeleteElementButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		// Target element must be in the project window
		if (!m_ElementPreviewArea->subviews.size() && m_ProjectWindow)
		{
			auto& ref = m_ProjectWindow->GetViewRef(m_SelectedTargetView.get());
			if (ref)
			{
				auto it = std::find(m_ProjectUIElements.begin(), m_ProjectUIElements.end(), ref);
				if (it != m_ProjectUIElements.end())
					m_ProjectUIElements.erase(it);

				m_ProjectWindow->RemoveView(ref);
			}

			if (m_PropertiesView->subviews.size())
				m_PropertiesView->subviews.clear();

			m_ProjectWindow->ForceUpdate(true);
		}
		else
		{
			// Target element is still in the element preview area
			if (m_PropertiesView->subviews.size())
				m_PropertiesView->subviews.clear();

			if (m_ElementPreviewArea->subviews.size())
				m_ElementPreviewArea->subviews.clear();
		}

		return EVENT_HANDLED;
	});
	m_WorkspaceTabView->GetTab("Editor")->AddSubview(m_DeleteElementButton);

#pragma endregion

#pragma region Editor Settings Area

	m_EditorSettingsView = MakeRef<UIView>();
	m_EditorSettingsView->layer.frame = Frame(Position{ 0, 0 }, Size{ 996, 440 });
	m_EditorSettingsView->layer.color = Color(51, 51, 52, 1.0f);
	m_WorkspaceTabView->GetTab("Settings")->AddSubview(m_EditorSettingsView);

	// Grid Snapping
	auto GridSnappingCheckbox = MakeRef<UICheckbox>();
	GridSnappingCheckbox->layer.frame = Frame(Position{ 55, 40 }, Size{ 280, 30 });
	GridSnappingCheckbox->Label->Text = "Enable Grid Snapping";
	GridSnappingCheckbox->Label->Properties.FontSize = 14;
	GridSnappingCheckbox->BoxSize = 16;
	GridSnappingCheckbox->layer.color = Color(88, 88, 89, 1);
	GridSnappingCheckbox->CheckedBoxColor = Color(108, 108, 109, 1);
	GridSnappingCheckbox->CheckmarkColor = Color::white;
	GridSnappingCheckbox->AddValueChangedEventHandler([this](bool checked, UICheckbox* sender) {
		utils::EditorSettings::GridSnapping = checked;
	});
	m_EditorSettingsView->AddSubview(GridSnappingCheckbox);

	// Grid Size
	auto GridSizeLabel = MakeRef<UILabel>(Frame(Position{ 60, 70 }, Size{ 160, 30 }));
	GridSizeLabel->Text = "Grid Size: ";
	GridSizeLabel->Properties.FontSize = 14;
	GridSizeLabel->Properties.Allignment = TextAlignment::LEADING;
	GridSizeLabel->color = Color::white;
	m_EditorSettingsView->AddSubview(GridSizeLabel);

	auto GridSizeInput = MakeRef<UITextbox>();
	GridSizeInput->layer.frame = Frame(Position{ 130, 75 }, Size{ 100, 20 });
	GridSizeInput->layer.color = Color(58, 58, 59, 1.0f);
	GridSizeInput->TextColor = Color::white;
	GridSizeInput->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	GridSizeInput->TextProperties.FontSize = 14;
	GridSizeInput->Placeholder = "Enter Value";
	GridSizeInput->Text = "10";
	utils::EditorSettings::GridSize = 10;
	GridSizeInput->AddEventHandler<EventType::KeyPressed>([GridSizeInput](Event& e, UIView* sender) -> bool {
		try {
			uint32_t value = std::stoi(GridSizeInput->Text);
			utils::EditorSettings::GridSize = value;
		}
		catch (...) {}

		return EVENT_HANDLED;
	});
	m_EditorSettingsView->AddSubview(GridSizeInput);

	// Auto-Saving
	auto AutoSavingCheckbox = MakeRef<UICheckbox>();
	AutoSavingCheckbox->layer.frame = Frame(Position{ 415, 40 }, Size{ 280, 30 });
	AutoSavingCheckbox->Label->Text = "Project Auto-Saving";
	AutoSavingCheckbox->Label->Properties.FontSize = 14;
	AutoSavingCheckbox->BoxSize = 16;
	AutoSavingCheckbox->layer.color = Color(88, 88, 89, 1);
	AutoSavingCheckbox->CheckedBoxColor = Color(108, 108, 109, 1);
	AutoSavingCheckbox->CheckmarkColor = Color::white;
	AutoSavingCheckbox->AddValueChangedEventHandler([this, AutoSavingCheckbox](bool checked, UICheckbox* sender) {
		std::thread autosaver_thread([this, AutoSavingCheckbox]() {
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			while (AutoSavingCheckbox->Checked)
			{
				std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
				if (std::chrono::duration_cast<std::chrono::seconds>(now - begin).count() >= utils::EditorSettings::ProjectAutoSavingInterval)
				{
					if (std::filesystem::is_directory(m_ProjectPathTextbox->Text) &&
						!m_UIClassNameTextbox->Text.empty() &&
						!m_ProjectNameTextbox->Text.empty() &&
						!m_ProjectPathTextbox->Text.empty())
					{
						GenerateProjectSolution(false);
					}
					begin = now;
				}

				Sleep(200);
			}
		});
		autosaver_thread.detach();
	});
	m_EditorSettingsView->AddSubview(AutoSavingCheckbox);

	// Auto-Saving Interval
	auto AutoSavingIntervalLabel = MakeRef<UILabel>(Frame(Position{ 380, 70 }, Size{ 180, 30 }));
	AutoSavingIntervalLabel->Text = "Auto-Saving Interval (sec.): ";
	AutoSavingIntervalLabel->Properties.FontSize = 14;
	AutoSavingIntervalLabel->Properties.Allignment = TextAlignment::LEADING;
	AutoSavingIntervalLabel->color = Color::white;
	m_EditorSettingsView->AddSubview(AutoSavingIntervalLabel);

	auto AutoSavingIntervalInput = MakeRef<UITextbox>();
	AutoSavingIntervalInput->layer.frame = Frame(Position{ 560, 75 }, Size{ 60, 20 });
	AutoSavingIntervalInput->layer.color = Color(58, 58, 59, 1.0f);
	AutoSavingIntervalInput->TextColor = Color::white;
	AutoSavingIntervalInput->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	AutoSavingIntervalInput->TextProperties.FontSize = 14;
	AutoSavingIntervalInput->Placeholder = "10";
	AutoSavingIntervalInput->Text = "10";
	utils::EditorSettings::ProjectAutoSavingInterval = 10;
	AutoSavingIntervalInput->AddEventHandler<EventType::KeyPressed>([AutoSavingIntervalInput](Event& e, UIView* sender) -> bool {
		try {
			uint32_t value = std::stoi(AutoSavingIntervalInput->Text);
			utils::EditorSettings::ProjectAutoSavingInterval = value;
		}
		catch (...) {}

		return EVENT_HANDLED;
	});
	m_EditorSettingsView->AddSubview(AutoSavingIntervalInput);

#pragma endregion

#pragma region Window Editing Area

	auto WindowEditingAreaLabel = MakeRef<UILabel>(Frame(Position{ 80, 60 }, Size{ 260, 40 }));
	WindowEditingAreaLabel->Text = "Project Window Settings";
	WindowEditingAreaLabel->Properties.FontSize = 18;
	WindowEditingAreaLabel->color = Color::white;
	m_EditorWindow->AddView(WindowEditingAreaLabel);

	// Window Width
	auto ProjectWindowWidthLabel = MakeRef<UILabel>(Frame(Position{ 80, 120 }, Size{ 160, 40 }));
	ProjectWindowWidthLabel->Text = "Window Width: ";
	ProjectWindowWidthLabel->Properties.FontSize = 14;
	ProjectWindowWidthLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectWindowWidthLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectWindowWidthLabel);

	m_ProjectWindowWidthTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 130 }, Size{ 140, 20 }));
	m_ProjectWindowWidthTextbox->Placeholder = "Enter width";
	m_ProjectWindowWidthTextbox->Text = "1000";
	m_ProjectWindowWidthTextbox->TextProperties.FontSize = 14;
	m_ProjectWindowWidthTextbox->CornerRadius = 2;
	m_ProjectWindowWidthTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectWindowWidthTextbox->TextColor = Color::white;
	m_ProjectWindowWidthTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectWindowWidthTextbox);

	// Window Height
	auto ProjectWindowHeightLabel = MakeRef<UILabel>(Frame(Position{ 80, 150 }, Size{ 160, 40 }));
	ProjectWindowHeightLabel->Text = "Window Height: ";
	ProjectWindowHeightLabel->Properties.FontSize = 14;
	ProjectWindowHeightLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectWindowHeightLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectWindowHeightLabel);

	m_ProjectWindowHeightTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 160 }, Size{ 140, 20 }));
	m_ProjectWindowHeightTextbox->Placeholder = "Enter height";
	m_ProjectWindowHeightTextbox->Text = "670";
	m_ProjectWindowHeightTextbox->TextProperties.FontSize = 14;
	m_ProjectWindowHeightTextbox->CornerRadius = 2;
	m_ProjectWindowHeightTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectWindowHeightTextbox->TextColor = Color::white;
	m_ProjectWindowHeightTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectWindowHeightTextbox);

	// Window Title
	auto ProjectWindowTitleLabel = MakeRef<UILabel>(Frame(Position{ 80, 180 }, Size{ 160, 40 }));
	ProjectWindowTitleLabel->Text = "Window Title: ";
	ProjectWindowTitleLabel->Properties.FontSize = 14;
	ProjectWindowTitleLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectWindowTitleLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectWindowTitleLabel);

	m_ProjectWindowTitleTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 190 }, Size{ 140, 20 }));
	m_ProjectWindowTitleTextbox->Placeholder = "Enter title";
	m_ProjectWindowTitleTextbox->Text = "Monochrome App";
	m_ProjectWindowTitleTextbox->TextProperties.FontSize = 14;
	m_ProjectWindowTitleTextbox->CornerRadius = 2;
	m_ProjectWindowTitleTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectWindowTitleTextbox->TextColor = Color::white;
	m_ProjectWindowTitleTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectWindowTitleTextbox);

	// Window Color
	auto ProjectWindowColorLabel = MakeRef<UILabel>(Frame(Position{ 80, 210 }, Size{ 160, 40 }));
	ProjectWindowColorLabel->Text = "Window Color: ";
	ProjectWindowColorLabel->Properties.FontSize = 14;
	ProjectWindowColorLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectWindowColorLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectWindowColorLabel);

	m_ProjectWindowColorTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 220 }, Size{ 140, 20 }));
	m_ProjectWindowColorTextbox->Placeholder = "100 250 255";
	m_ProjectWindowColorTextbox->Text = utils::ColorToString(Color::gray);
	m_ProjectWindowColorTextbox->TextProperties.FontSize = 14;
	m_ProjectWindowColorTextbox->CornerRadius = 2;
	m_ProjectWindowColorTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectWindowColorTextbox->TextColor = Color::white;
	m_ProjectWindowColorTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectWindowColorTextbox);

	m_OpenProjectWindowButton = MakeRef<UIButton>(Frame(Position{ 120, 270 }, Size{180, 36}));
	m_OpenProjectWindowButton->Label->Text = "Open Window";
	m_OpenProjectWindowButton->Label->color = Color::white;
	m_OpenProjectWindowButton->Label->Properties.FontSize = 14;
	m_OpenProjectWindowButton->layer.color = Color(49, 49, 50, 1.0f);
	m_OpenProjectWindowButton->CornerRadius = 4;
	m_OpenProjectWindowButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
			OpenProjectWindow();

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_OpenProjectWindowButton);

#pragma endregion

#pragma region Project Saving Area

	auto ProjectEditingAreaLabel = MakeRef<UILabel>(Frame(Position{ 80, 540 }, Size{ 260, 40 }));
	ProjectEditingAreaLabel->Text = "Project Settings";
	ProjectEditingAreaLabel->Properties.FontSize = 18;
	ProjectEditingAreaLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectEditingAreaLabel);

	// Project Name
	auto ProjectNameLabel = MakeRef<UILabel>(Frame(Position{ 80, 600 }, Size{ 160, 40 }));
	ProjectNameLabel->Text = "Project Name: ";
	ProjectNameLabel->Properties.FontSize = 14;
	ProjectNameLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectNameLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectNameLabel);

	m_ProjectNameTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 610 }, Size{ 160, 20 }));
	m_ProjectNameTextbox->Placeholder = "Enter project name";
	m_ProjectNameTextbox->Text = "Project 1";
	m_ProjectNameTextbox->TextProperties.FontSize = 14;
	m_ProjectNameTextbox->CornerRadius = 2;
	m_ProjectNameTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectNameTextbox->TextColor = Color::white;
	m_ProjectNameTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectNameTextbox);

	// Project Path
	auto ProjectPathLabel = MakeRef<UILabel>(Frame(Position{ 80, 630 }, Size{ 160, 40 }));
	ProjectPathLabel->Text = "Project Location: ";
	ProjectPathLabel->Properties.FontSize = 14;
	ProjectPathLabel->Properties.Allignment = TextAlignment::LEADING;
	ProjectPathLabel->color = Color::white;
	m_EditorWindow->AddView(ProjectPathLabel);

	m_ProjectPathTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 640 }, Size{ 160, 20 }));
	m_ProjectPathTextbox->Placeholder = "Project path";
	m_ProjectPathTextbox->Text = "";
	m_ProjectPathTextbox->TextProperties.FontSize = 14;
	m_ProjectPathTextbox->CornerRadius = 2;
	m_ProjectPathTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_ProjectPathTextbox->TextColor = Color::white;
	m_ProjectPathTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_ProjectPathTextbox);

	m_SelectProjectPathButton = MakeRef<UIButton>(Frame(Position{ 230, 665 }, Size{ 120, 20 }));
	m_SelectProjectPathButton->Label->Text = "Select ...";
	m_SelectProjectPathButton->Label->color = Color::white;
	m_SelectProjectPathButton->Label->Properties.FontSize = 12;
	m_SelectProjectPathButton->layer.color = Color(49, 49, 50, 1.0f);
	m_SelectProjectPathButton->CornerRadius = 2;
	m_SelectProjectPathButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
		{
			UIFileDialogue fd;
			auto path = fd.ChooseDirectoryDialogue();

			if (!path.empty())
			{
				m_ProjectPathTextbox->Text = path;
				m_ProjectPathTextbox->TextColor = Color::white;
			}
		}

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_SelectProjectPathButton);

	// UI Class Name
	auto UIClassNameLabel = MakeRef<UILabel>(Frame(Position{ 80, 695 }, Size{ 160, 40 }));
	UIClassNameLabel->Text = "UI Class Name: ";
	UIClassNameLabel->Properties.FontSize = 14;
	UIClassNameLabel->Properties.Allignment = TextAlignment::LEADING;
	UIClassNameLabel->color = Color::white;
	m_EditorWindow->AddView(UIClassNameLabel);

	m_UIClassNameTextbox = MakeRef<UITextbox>(Frame(Position{ 190, 705 }, Size{ 160, 20 }));
	m_UIClassNameTextbox->Placeholder = "Name";
	m_UIClassNameTextbox->Text = "UserInterface";
	m_UIClassNameTextbox->TextProperties.FontSize = 14;
	m_UIClassNameTextbox->CornerRadius = 2;
	m_UIClassNameTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_UIClassNameTextbox->TextColor = Color::white;
	m_UIClassNameTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_UIClassNameTextbox);

	m_GenerateSourceAndVSSolution = MakeRef<UIButton>(Frame(Position{ 120, 770 }, Size{ 200, 34 }));
	m_GenerateSourceAndVSSolution->Label->Text = "Generate Source and VS Project";
	m_GenerateSourceAndVSSolution->Label->color = Color::white;
	m_GenerateSourceAndVSSolution->Label->Properties.FontSize = 12;
	m_GenerateSourceAndVSSolution->layer.color = Color(49, 49, 50, 1.0f);
	m_GenerateSourceAndVSSolution->CornerRadius = 4;
	m_GenerateSourceAndVSSolution->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
			GenerateProjectSolution(true);

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_GenerateSourceAndVSSolution);

	m_GenerateProjectSourceFiles = MakeRef<UIButton>(Frame(Position{ 120, 816 }, Size{ 200, 34 }));
	m_GenerateProjectSourceFiles->Label->Text = "Generate Source Files";
	m_GenerateProjectSourceFiles->Label->color = Color::white;
	m_GenerateProjectSourceFiles->Label->Properties.FontSize = 12;
	m_GenerateProjectSourceFiles->layer.color = Color(49, 49, 50, 1.0f);
	m_GenerateProjectSourceFiles->CornerRadius = 4;
	m_GenerateProjectSourceFiles->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
			GenerateProjectSolution();

		return EVENT_HANDLED;
		});
	m_EditorWindow->AddView(m_GenerateProjectSourceFiles);

	m_MonochromeSourcePathTextbox = MakeRef<UITextbox>(Frame(Position{ 50, 370 }, Size{ 240, 20 }));
	m_MonochromeSourcePathTextbox->Placeholder = "Monochrome Source Path";
	m_MonochromeSourcePathTextbox->Text = "";
	m_MonochromeSourcePathTextbox->TextProperties.FontSize = 14;
	m_MonochromeSourcePathTextbox->CornerRadius = 2;
	m_MonochromeSourcePathTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_MonochromeSourcePathTextbox->TextColor = Color::white;
	m_MonochromeSourcePathTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_MonochromeSourcePathTextbox);

	m_SelectMonochromeSourcePathButton = MakeRef<UIButton>(Frame(Position{ 300, 370 }, Size{ 100, 20 }));
	m_SelectMonochromeSourcePathButton->Label->Text = "Select ...";
	m_SelectMonochromeSourcePathButton->Label->color = Color::white;
	m_SelectMonochromeSourcePathButton->Label->Properties.FontSize = 12;
	m_SelectMonochromeSourcePathButton->layer.color = Color(49, 49, 50, 1.0f);
	m_SelectMonochromeSourcePathButton->CornerRadius = 2;
	m_SelectMonochromeSourcePathButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
		{
			UIFileDialogue fd;
			auto path = fd.ChooseDirectoryDialogue();

			if (!path.empty())
			{
				m_MonochromeSourcePathTextbox->Text = path;
				m_MonochromeSourcePathTextbox->TextColor = Color::white;
				SaveConfiguration();
			}
		}

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_SelectMonochromeSourcePathButton);

	m_MonochromeLibDbgPathTextbox = MakeRef<UITextbox>(Frame(Position{ 50, 400 }, Size{ 240, 20 }));
	m_MonochromeLibDbgPathTextbox->Placeholder = "Monochrome Library Debug Path";
	m_MonochromeLibDbgPathTextbox->Text = "";
	m_MonochromeLibDbgPathTextbox->TextProperties.FontSize = 14;
	m_MonochromeLibDbgPathTextbox->CornerRadius = 2;
	m_MonochromeLibDbgPathTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_MonochromeLibDbgPathTextbox->TextColor = Color::white;
	m_MonochromeLibDbgPathTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_MonochromeLibDbgPathTextbox);

	m_SelectMonochromeLibDbgPathButton = MakeRef<UIButton>(Frame(Position{ 300, 400 }, Size{ 100, 20 }));
	m_SelectMonochromeLibDbgPathButton->Label->Text = "Select ...";
	m_SelectMonochromeLibDbgPathButton->Label->color = Color::white;
	m_SelectMonochromeLibDbgPathButton->Label->Properties.FontSize = 12;
	m_SelectMonochromeLibDbgPathButton->layer.color = Color(49, 49, 50, 1.0f);
	m_SelectMonochromeLibDbgPathButton->CornerRadius = 2;
	m_SelectMonochromeLibDbgPathButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
		{
			UIFileDialogue fd;
			auto path = fd.ChooseDirectoryDialogue();

			if (!path.empty())
			{
				m_MonochromeLibDbgPathTextbox->Text = path;
				m_MonochromeLibDbgPathTextbox->TextColor = Color::white;
				SaveConfiguration();
			}
		}

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_SelectMonochromeLibDbgPathButton);

	m_MonochromeLibRelPathTextbox = MakeRef<UITextbox>(Frame(Position{ 50, 430 }, Size{ 240, 20 }));
	m_MonochromeLibRelPathTextbox->Placeholder = "Monochrome Library Release Path";
	m_MonochromeLibRelPathTextbox->Text = "";
	m_MonochromeLibRelPathTextbox->TextProperties.FontSize = 14;
	m_MonochromeLibRelPathTextbox->CornerRadius = 2;
	m_MonochromeLibRelPathTextbox->layer.color = Color(58, 58, 59, 1.0f);
	m_MonochromeLibRelPathTextbox->TextColor = Color::white;
	m_MonochromeLibRelPathTextbox->FocusedHighlightColor = Color(28, 28, 29, 1.0f);
	m_EditorWindow->AddView(m_MonochromeLibRelPathTextbox);

	m_SelectMonochromeLibRelPathButton = MakeRef<UIButton>(Frame(Position{ 300, 430 }, Size{ 100, 20 }));
	m_SelectMonochromeLibRelPathButton->Label->Text = "Select ...";
	m_SelectMonochromeLibRelPathButton->Label->color = Color::white;
	m_SelectMonochromeLibRelPathButton->Label->Properties.FontSize = 12;
	m_SelectMonochromeLibRelPathButton->layer.color = Color(49, 49, 50, 1.0f);
	m_SelectMonochromeLibRelPathButton->CornerRadius = 2;
	m_SelectMonochromeLibRelPathButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
		{
			UIFileDialogue fd;
			auto path = fd.ChooseDirectoryDialogue();

			if (!path.empty())
			{
				m_MonochromeLibRelPathTextbox->Text = path;
				m_MonochromeLibRelPathTextbox->TextColor = Color::white;
				SaveConfiguration();
			}
		}

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_SelectMonochromeLibRelPathButton);

#pragma endregion

#pragma region Project Loading Area 

	// TEMP: This is going to move into the FileMenu once it exists
	Ref<UIButton> SelectFileButton = MakeRef<UIButton>();
	SelectFileButton->layer.frame = Frame({ 120, 860 }, { 200, 34 });
	SelectFileButton->Label->Text = "Load Project";
	SelectFileButton->Label->color = Color::white;
	SelectFileButton->Label->Properties.FontSize = 12;
	SelectFileButton->layer.color = Color(49, 49, 50, 1.0f);
	SelectFileButton->CornerRadius = 4;
	SelectFileButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event &evt, UIView *sender) -> bool
	{
		UIFileDialogue fd;

		// Set Filter, only mc Files are valid for now
		UIFileDialogueFilter filter;
		filter.AddFilter(L"FileType", L"*.mc");
		fd.SetFilter(filter);

		// Load the file and then load the project, if file is loaded 
		auto path = fd.ChooseFileDialogue();
		if (!path.empty())
		{
			// File is valid, load the project
			utils::MCLayout layout = utils::ProjectGenerator::LoadMCProject(path);
			
			// Set the project window properties
			m_ProjectWindowWidth = layout.windowSettings.width;
			m_ProjectWindowWidthTextbox->Text = std::to_string(layout.windowSettings.width);

			m_ProjectWindowHeight = layout.windowSettings.height;
			m_ProjectWindowHeightTextbox->Text = std::to_string(layout.windowSettings.height);

			m_ProjectWindowTitleTextbox->Text = layout.windowSettings.title;

			std::stringstream textBoxColor;
			textBoxColor << layout.windowSettings.color.r << " " << layout.windowSettings.color.g << " " << layout.windowSettings.color.b << " " << layout.windowSettings.color.alpha;
			m_ProjectWindowColorTextbox->Text = textBoxColor.str();

			// Set the UI Elements
			m_ProjectUIElements.clear();

			OpenProjectWindow();

			// Waiting for the window to open
			while (!m_ProjectWindow || !m_ProjectWindow->IsOpened())
				Sleep(20);
			
			// Clears all existing views
			m_ProjectWindow->RemoveAllViews();

			for (auto& view : layout.uiViews)
				AddElementToProjectWindow(view);
		}

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(SelectFileButton);

#pragma endregion

}

void MonochromeEditor::OpenElementProperties(Ref<UIView> TargetElement)
{
	if (!TargetElement) return;
	
	m_PropertiesView->subviews.clear();
	m_VariableCodeProperties.RegisterElement(TargetElement);

	if (utils::CheckType<UILabel>(TargetElement.get())) { return m_LabelProperties.Open(std::dynamic_pointer_cast<UILabel>(TargetElement)); }
	if (utils::CheckType<UIButton>(TargetElement.get())) { return m_ButtonProperties.Open(std::dynamic_pointer_cast<UIButton>(TargetElement)); }

	// If it doesn't belong to any other category, open plain view properties.
	m_PlainViewProperties.Open(TargetElement);
}

void MonochromeEditor::OpenProjectWindow()
{
	uint32_t width = 1000, height = 500;

	try { width = std::stoi(m_ProjectWindowWidthTextbox->Text); m_ProjectWindowWidth = width; }
	catch (...) {}
	
	try { height = std::stoi(m_ProjectWindowHeightTextbox->Text); m_ProjectWindowHeight = height; }
	catch (...) {}

	if (!m_ProjectWindow)
	{
		std::thread project_window_thread([this, width, height]() {
			Color BackgroundColor = utils::StringToColor(m_ProjectWindowColorTextbox->Text);

			m_ProjectWindow = UIWindow::Create(WindowStyle::Modern, width, height, m_ProjectWindowTitleTextbox->Text.c_str());
			m_ProjectWindow->SetBackgroundColor(BackgroundColor);
			m_ProjectWindow->SetModernWindowButtonsColor(BackgroundColor);

			Ref<UIView> BackgroundEventView = MakeRef<UIView>(Frame(0, 0, (float)width, (float)height));
			BackgroundEventView->layer.color = Color::transparent;
			BackgroundEventView->SetZIndex(0);
			BackgroundEventView->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
				if (!m_ElementPreviewArea->subviews.size())
					m_PropertiesView->subviews.clear();

				m_EditorWindow->ForceUpdate(true);
				return EVENT_HANDLED;
			});
			m_ProjectWindow->AddView(BackgroundEventView);

			BackgroundEventView->AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) -> bool {
				m_PWScriptController.Widget_OnMouseReleased();
				return EVENT_HANDLED;
			});

			for (auto& view : m_ProjectUIElements)
			{
				view->SetZIndex(1);
				m_ProjectWindow->AddView(view);
			}

			m_ProjectWindow->GetCloseButtonRef()->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
				m_PropertiesView->subviews.clear();
				m_EditorWindow->ForceUpdate(true);
				return EVENT_HANDLED;
			});

			m_ProjectWindow->StartWindowLoop();
		});
		project_window_thread.detach();
	}
	else if (!m_ProjectWindow->IsOpened())
	{
		m_ProjectWindow = nullptr;
		OpenProjectWindow();
	}
}

void MonochromeEditor::AddElementToProjectWindow(Ref<UIView> elem)
{
	Ref<UIView> TargetElement = nullptr;
	if (!elem)
	{
		if (!m_ElementPreviewArea->subviews.size() || !m_ProjectWindow || !m_ProjectWindow->IsOpened())
			return;

		TargetElement = m_ElementPreviewArea->subviews.at(0);
	}
	else
		TargetElement = elem;

	TargetElement->parent = nullptr;
	TargetElement->SetZIndex(1);

	// Setting an event handler so that elements can be drageged with a mouse inside the project window
	TargetElement->AddEventHandler<EventType::MouseButtonPressed>([this](Event& e, UIView* sender) -> bool {
		Ref<UIView> target = m_ProjectWindow->GetViewRef(sender);
		m_PWScriptController.FindInnerMostView(target, target);
		m_PWScriptController.Widget_OnMousePressed(target);

		return EVENT_HANDLED;
	});

	TargetElement->AddEventHandler<EventType::MouseButtonReleased>([this](Event& e, UIView* sender) -> bool {
		Ref<UIView> target = m_ProjectWindow->GetViewRef(sender);
		m_PWScriptController.Widget_OnMouseReleased();

		return EVENT_HANDLED;
	});

	TargetElement->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		MouseButtonClickedEvent& mbc = reinterpret_cast<MouseButtonClickedEvent&>(e);
		if (mbc.button == MouseButton::Left)
		{
			if (!m_ElementPreviewArea->subviews.size())
			{
				auto& ref = m_ProjectWindow->GetViewRef(sender);
				if (!ref) 
					return EVENT_HANDLED;

				Ref<UIView> target = ref; 
				m_PWScriptController.FindInnerMostView(ref, target);

				m_OpenVariablePropertiesButton->Label->Text = "Variable Properties";
				OpenElementProperties(target);
				m_SelectedTargetView = target;

				m_EditorWindow->ForceUpdate(true);
			}
		}

		return EVENT_HANDLED;
	});

	TargetElement->AddEventHandler<EventType::MouseMoved>([this](Event& e, UIView* sender) -> bool {
		if (!sender)
			return EVENT_HANDLED;

		auto pt = sender->srcwindow->GetMouseCursorPos() - sender->GetAbsolutePosition();
		const auto CheckForResizingConditions = [sender, pt]() -> bool {
			static float VerticalBorderWidth = 10.0f;
			static float HorizontalBorderWidth = 20.0f;

			/*top-left, top and top-right*/
			if (pt.y < VerticalBorderWidth)
			{
				if (pt.x < HorizontalBorderWidth)
				{
					UICursor::SetCursor(CursorType::SizeNWE);
					return true;
				}
				else if (pt.x > (sender->layer.frame.size.width - HorizontalBorderWidth))
				{
					UICursor::SetCursor(CursorType::SizeNESW);
					return true;
				}
				UICursor::SetCursor(CursorType::SizeNS);
				return true;
			}
			/*bottom-left, bottom and bottom-right */
			if (pt.y > (sender->layer.frame.size.height - VerticalBorderWidth))
			{
				if (pt.x < HorizontalBorderWidth)
				{
					UICursor::SetCursor(CursorType::SizeNESW);
					return true;
				}
				else if (pt.x > (sender->layer.frame.size.width - HorizontalBorderWidth))
				{
					UICursor::SetCursor(CursorType::SizeNWE);
					return true;
				}

				UICursor::SetCursor(CursorType::SizeNS);
				return true;
			}
			if (pt.x < HorizontalBorderWidth)
			{
				UICursor::SetCursor(CursorType::SizeWE);
				return true;
			}
			if (pt.x > (sender->layer.frame.size.width - HorizontalBorderWidth))
			{
				UICursor::SetCursor(CursorType::SizeWE);
				return true;
			}

			return false;
		};

		if (!CheckForResizingConditions())
			UICursor::SetCursor(CursorType::Arrow);

		return EVENT_HANDLED;
	});

	m_ElementPreviewArea->subviews.clear();
	m_PropertiesView->subviews.clear();

	m_ProjectUIElements.push_back(TargetElement);
	m_ProjectWindow->AddView(TargetElement);
	m_ProjectWindow->ForceUpdate(true);
}

void MonochromeEditor::GenerateProjectSolution(bool run_cmake)
{
	// Checking for a valid Project Name
	if (m_ProjectNameTextbox->Text.empty()) { m_ProjectNameTextbox->TextColor = Color(155, 28, 29, 1.0f); return; }
	else { m_ProjectNameTextbox->TextColor = Color::white; }

	// Checking for a valid Class Name
	if (m_UIClassNameTextbox->Text.empty()) { m_UIClassNameTextbox->TextColor = Color(155, 28, 29, 1.0f); return; }
	else { m_UIClassNameTextbox->TextColor = Color::white; }

	// Checking monochrome source path
	std::string MonochromeSourcePath = m_MonochromeSourcePathTextbox->Text;
	if (!std::filesystem::is_directory(MonochromeSourcePath)) { m_MonochromeSourcePathTextbox->TextColor = Color(155, 28, 29, 1.0f); return; }
	else { m_MonochromeSourcePathTextbox->TextColor = Color::white; }

	// Checking monochrome library debug path
	std::string MonochromeLibDbgPath = m_MonochromeLibDbgPathTextbox->Text;
	if (!std::filesystem::is_directory(MonochromeLibDbgPath)) { m_MonochromeLibDbgPathTextbox->TextColor = Color(155, 28, 29, 1.0f); return; }
	else { m_MonochromeLibDbgPathTextbox->TextColor = Color::white; }

	// Checking monochrome library release path
	std::string MonochromeLibRelPath = m_MonochromeLibRelPathTextbox->Text;
	if (!std::filesystem::is_directory(MonochromeLibRelPath)) { m_MonochromeLibRelPathTextbox->TextColor = Color(155, 28, 29, 1.0f); return; }
	else { m_MonochromeLibRelPathTextbox->TextColor = Color::white; }

	if (std::filesystem::is_directory(m_ProjectPathTextbox->Text))
	{
		if (run_cmake)
		{
			if (AllocConsole()) {
#pragma warning ( suppress: 4996 )
				freopen("CONOUT$", "wt", stdout);
				SetConsoleTitle(L"Building Monochrome Project");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			}
		}

		utils::WindowSettings ws;
		ws.color = utils::StringToColor(m_ProjectWindowColorTextbox->Text);
		ws.title = m_ProjectWindowTitleTextbox->Text.c_str();
		ws.width = m_ProjectWindowWidth;
		ws.height = m_ProjectWindowHeight;

		utils::ProjectConfig config;
		config.windowSettings = ws;
		config.location = m_ProjectPathTextbox->Text;
		config.uiClassName = m_UIClassNameTextbox->Text;
		config.projectName = m_ProjectNameTextbox->Text;
		config.uiViews = m_ProjectUIElements;

		config.monochromeSourcePath = MonochromeSourcePath;
		config.monochromeLibraryDebugPath = MonochromeLibDbgPath;
		config.monochromeLibraryReleasePath = MonochromeLibRelPath;

		config.elementCodeProperties = m_VariableCodeProperties.GetRegisteredElementCodeProperties();

		if (run_cmake)
			utils::ProjectGenerator::GenerateProjectAndVisualStudioSolution(config);
		else
			utils::ProjectGenerator::GenerateProjectSourceFiles(config);

		if (run_cmake)
		{
			HWND console = GetConsoleWindow();
			ShowWindow(console, SW_HIDE);
			FreeConsole();
		}
	}
	else
	{
		m_ProjectPathTextbox->TextColor = Color(155, 28, 29, 1.0f);
	}
}
