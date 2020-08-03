#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include "Monochrome.h"
using namespace mc;

void StartSecondWindow()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	auto ChildWindow = UIWindow::Create(WindowStyle::Modern, 800, 470, "Child Window");
	ChildWindow->SetBackgroundColor(Color(28, 61, 31, 1.0f));
	ChildWindow->SetModernWindowButtonsColor(Color(28, 61, 31, 1.0f));

	Ref<UIButton> button = MakeRef<UIButton>();
	button->layer.frame = Frame({ 280, 120 }, { 200, 36 });
	button->Label->Text = "Child Button";
	button->AddEventHandler<EventType::MouseButtonClicked>([button](Event& evt, UIView* sender) -> bool {
		button->Label->Text = "You Clicked Me :)";
		return EVENT_HANDLED;
	});
	ChildWindow->AddView(button);

	ChildWindow->StartWindowLoop();
}

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "Parent Window");
	window->SetBackgroundColor(Color(28, 21, 31, 1.0f));
	window->SetModernWindowButtonsColor(Color(28, 21, 31, 1.0f));

	Ref<UIButton> button = MakeRef<UIButton>();
	button->layer.frame = Frame({ 280, 120 }, { 200, 36 });
	button->Label->Text = "Click Me";
	button->AddEventHandler<EventType::MouseButtonClicked>([](Event& evt, UIView* sender) -> bool {
		MessageBoxA(0, "Clicked", "UIButton", 0);
		return EVENT_HANDLED;
	});
	window->AddView(button);

	std::thread child_window_thread(&StartSecondWindow);
	child_window_thread.detach();

	window->StartWindowLoop();
    return 0;
}
