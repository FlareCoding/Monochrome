#include "Editor.h"
#include <ShlObj.h>
#include <filesystem>

void MonochromeEditor::Start()
{
	m_EditorWindow->StartWindowLoop();
}

void MonochromeEditor::Initialize()
{
	CreateEditorWindow();
	InitEditorUI();

	m_LabelProperties.Initialize(m_ElementPreviewArea, m_PropertiesView);
	m_ButtonProperties.Initialize(m_ElementPreviewArea, m_PropertiesView);

	m_PropertiesView->subviews.clear();
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

		ToolboxWidgetButton->AddEventHandler<EventType::MouseButtonClicked>([click_cb](Event& e, UIView* sender) -> bool {
			click_cb();
			return EVENT_HANDLED;
		});

		m_Toolbox->AddChild(ToolboxWidgetButton);
		PreviousPosY += 62.0f;

		return ToolboxWidgetButton;
	};

	static Position DefaultElementPreviewPosition = Position{ 360, 200 };

	auto ToolboxWidgetButton_Label = MakeToolboxWidgetButton("Label", [this]() {
		m_ElementPreviewArea->subviews.clear();

		Ref<UILabel> widget = MakeRef<UILabel>();
		widget->color = Color::white;
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);

		OpenElementProperties(Widget::Label);
	});

	auto ToolboxWidgetButton_Button	= MakeToolboxWidgetButton("Button", [this]() {
		m_ElementPreviewArea->subviews.clear();

		Ref<UIButton> widget = MakeRef<UIButton>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);

		OpenElementProperties(Widget::Button);
	});

	auto ToolboxWidgetButton_Checkbox = MakeToolboxWidgetButton("Checkbox", [this]() {
		m_ElementPreviewArea->subviews.clear();

		Ref<UICheckbox> widget = MakeRef<UICheckbox>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		m_ElementPreviewArea->AddSubview(widget);

		OpenElementProperties(Widget::Checkbox);
	});
	
	auto ToolboxWidgetButton_Slider	= MakeToolboxWidgetButton("Slider", [this]() {
		m_ElementPreviewArea->subviews.clear();

		Ref<UISlider> widget = MakeRef<UISlider>();
		widget->layer.frame.position = DefaultElementPreviewPosition;
		widget->SliderKnobShape = Shape::Circle;
		widget->VisibleTickmarks = false;
		m_ElementPreviewArea->AddSubview(widget);

		OpenElementProperties(Widget::Slider);
	});

	m_EditorWindow->AddView(m_Toolbox);

#pragma endregion

#pragma region Element Preview Area

	m_ElementPreviewArea = MakeRef<UIView>();
	m_ElementPreviewArea->layer.frame = Frame(Position{ (float)m_EditorWidth/2.0f - 450, (float)m_EditorHeight - 450 }, Size{ 900, 448 });
	m_ElementPreviewArea->layer.color = Color(51, 51, 52, 1.0f);
	m_EditorWindow->AddView(m_ElementPreviewArea);

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
	m_EditorWindow->AddView(m_AddElementToProjectWindowButton);

#pragma endregion

#pragma region Element Properties Area

	m_PropertiesView = MakeRef<UIView>();
	m_PropertiesView->layer.frame = Frame(Position{ (float)m_EditorWidth / 2.0f - 500, 60 }, Size{ 1000, 440 });
	m_PropertiesView->layer.color = Color(51, 51, 52, 1.0f);
	m_EditorWindow->AddView(m_PropertiesView);

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
			char szDir[MAX_PATH];
			BROWSEINFOA bInfo;
			bInfo.hwndOwner = NULL;
			bInfo.pidlRoot = NULL;
			bInfo.pszDisplayName = szDir;
			bInfo.lpszTitle = "Choose location";
			bInfo.ulFlags = 0;
			bInfo.lpfn = NULL;
			bInfo.lParam = 0;
			bInfo.iImage = -1;

			LPITEMIDLIST lpItem = SHBrowseForFolderA(&bInfo);
			std::string dir = "C:\\";

			if (lpItem != NULL)
			{
				SHGetPathFromIDListA(lpItem, szDir);
				m_ProjectPathTextbox->Text = std::string(szDir);
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

	m_GenerateProjectButton = MakeRef<UIButton>(Frame(Position{ 120, 770 }, Size{ 200, 30 }));
	m_GenerateProjectButton->Label->Text = "Generate Project";
	m_GenerateProjectButton->Label->color = Color::white;
	m_GenerateProjectButton->Label->Properties.FontSize = 12;
	m_GenerateProjectButton->layer.color = Color(49, 49, 50, 1.0f);
	m_GenerateProjectButton->CornerRadius = 4;
	m_GenerateProjectButton->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		if (((MouseButtonClickedEvent&)e).button == MouseButton::Left)
			GenerateProjectSolution();

		return EVENT_HANDLED;
	});
	m_EditorWindow->AddView(m_GenerateProjectButton);

#pragma endregion

}

void MonochromeEditor::OpenElementProperties(Widget type)
{
	m_PropertiesView->subviews.clear();

	switch (type)
	{
	case Widget::Label:
	{
		m_LabelProperties.Open(std::dynamic_pointer_cast<UILabel>(m_ElementPreviewArea->subviews.at(0)));
		break;
	}
	case Widget::Button:
	{
		m_ButtonProperties.Open(std::dynamic_pointer_cast<UIButton>(m_ElementPreviewArea->subviews.at(0)));
		break;
	}
	case Widget::Checkbox:
	{
		break;
	}
	case Widget::Slider:
	{
		break;
	}
	default: break;
	}
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

			for (auto& view : m_ProjectUIElements)
				m_ProjectWindow->AddView(view);

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

void MonochromeEditor::AddElementToProjectWindow()
{
	if (!m_ElementPreviewArea->subviews.size() || !m_ProjectWindow || !m_ProjectWindow->IsOpened()) return;

	Ref<UIView> TargetElement = m_ElementPreviewArea->subviews.at(0);
	TargetElement->parent = nullptr;

	// Setting an event handler so that elements can be drageged with a mouse inside the project window
	TargetElement->AddEventHandler<EventType::MouseMoved>([](Event& e, UIView* sender) -> bool {
		MouseMovedEvent& mme = reinterpret_cast<MouseMovedEvent&>(e);
		if (mme.button == MouseButton::Left)
		{
			float multiplier = 1; // to be adjusted in future commits

			Point distance;
			distance.x = mme.distance.x * multiplier;
			distance.y = mme.distance.y * multiplier;

			sender->layer.frame.position += distance;
		}

		return EVENT_HANDLED;
	});

	TargetElement->AddEventHandler<EventType::MouseButtonClicked>([this](Event& e, UIView* sender) -> bool {
		MouseButtonClickedEvent& mbc = reinterpret_cast<MouseButtonClickedEvent&>(e);
		if (mbc.button == MouseButton::Right)
		{
			// Remove target from project window and bring it back to the element preview panel
		}

		return EVENT_HANDLED;
	});

	m_ElementPreviewArea->subviews.clear();
	m_PropertiesView->subviews.clear();

	m_ProjectUIElements.push_back(TargetElement);
	m_ProjectWindow->AddView(TargetElement);
}

void MonochromeEditor::GenerateProjectSolution()
{
	if (m_UIClassNameTextbox->Text.empty())
	{
		m_UIClassNameTextbox->TextColor = Color(155, 28, 29, 1.0f);
		return;
	}
	else
		m_UIClassNameTextbox->TextColor = Color::white;

	if (std::filesystem::is_directory(m_ProjectPathTextbox->Text))
	{
		if (AllocConsole()) {
#pragma warning ( suppress: 4996 )
			freopen("CONOUT$", "wt", stdout);
			SetConsoleTitle(L"Building Monochrome Project");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}

		m_GenerateProjectButton->Visible = false;

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

		utils::ProjectGenerator::GenerateProject(config);

		HWND console = GetConsoleWindow();
		ShowWindow(console, SW_HIDE);
		FreeConsole();
		m_GenerateProjectButton->Visible = true;
	}
	else
	{
		m_ProjectPathTextbox->TextColor = Color(155, 28, 29, 1.0f);
	}
}
