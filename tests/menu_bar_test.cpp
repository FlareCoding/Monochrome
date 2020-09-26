#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <Monochrome.h>
#include <iostream>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "MenuBar Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));

	Ref<FileMenu> fileMenu = FileMenu::Create("File");
	fileMenu->AddMenuItem("New Project", []() { std::cout << "New Project Callback" << std::endl; });
	fileMenu->AddMenuSeperator();
	fileMenu->AddMenuItem("Open Project", []() { std::cout << "Open Project Callback" << std::endl; });
	fileMenu->AddMenuItem("Save Project", []() { std::cout << "Save Project Callback" << std::endl; });
	fileMenu->AddMenuItem("Save Project as..", []() { std::cout << "Save Project as Callback" << std::endl; });
	fileMenu->AddMenuSeperator();
	fileMenu->AddMenuItem("Project Settings", []() { std::cout << "Project Settings Callback" << std::endl; });

	Ref<FileMenu> helpMenu = FileMenu::Create("Help");
	helpMenu->AddMenuItem("About Monochrome", []() { std::cout << "About Callback" << std::endl; });
	helpMenu->AddMenuSeperator();
	helpMenu->AddMenuItem("Check for Updates...", []() { std::cout << "Check for Updates Callback" << std::endl; });
	helpMenu->AddMenuItem("Go to Documentation", []() { std::cout << "Documentation Callback" << std::endl; });
	helpMenu->AddMenuItem("Go to Forum", []() { std::cout << "Forum Callback" << std::endl; });

	Ref<FileMenuBar> menubar = FileMenuBar::Create();
	menubar->AddMenu(fileMenu);
	menubar->AddMenu(helpMenu);
	window->SetMenuBar(menubar);

	window->StartWindowLoop();
}