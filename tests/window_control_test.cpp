#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <Monochrome.h>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "Window Control Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));

	Ref<UIButton> ButtonResizeUp = MakeRef<UIButton>();
	ButtonResizeUp->layer.frame = Frame({ 200, 120 }, { 260, 36 });
	ButtonResizeUp->Label->Text = "window->SetSize(1270, 720)";
	ButtonResizeUp->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		window->SetSize(1270, 720);
		return EVENT_HANDLED;
	});
	window->AddView(ButtonResizeUp);

	Ref<UIButton> ButtonResizeDown = MakeRef<UIButton>();
	ButtonResizeDown->layer.frame = Frame({ 200, 180 }, { 260, 36 });
	ButtonResizeDown->Label->Text = "window->SetSize(1000, 670)";
	ButtonResizeDown->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		window->SetSize(1000, 670);
		return EVENT_HANDLED;
	});
	window->AddView(ButtonResizeDown);

	Ref<UIButton> ButtonMoveLeft = MakeRef<UIButton>();
	ButtonMoveLeft->layer.frame = Frame({ 500, 120 }, { 260, 36 });
	ButtonMoveLeft->Label->Text = "window->SetPos(40, 40)";
	ButtonMoveLeft->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		window->SetPos(40, 40);
		return EVENT_HANDLED;
	});
	window->AddView(ButtonMoveLeft);

	Ref<UIButton> ButtonMoveRight = MakeRef<UIButton>();
	ButtonMoveRight->layer.frame = Frame({ 500, 180 }, { 260, 36 });
	ButtonMoveRight->Label->Text = "window->SetPos(400, 200)";
	ButtonMoveRight->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		window->SetPos(400, 200);
		return EVENT_HANDLED;
	});
	window->AddView(ButtonMoveRight);

	Ref<UIButton> ButtonChangeTitle = MakeRef<UIButton>();
	ButtonChangeTitle->layer.frame = Frame({ 350, 240 }, { 260, 36 });
	ButtonChangeTitle->Label->Text = "window->SetTitle(new title)";
	ButtonChangeTitle->AddEventHandler<EventType::MouseButtonClicked>([window](Event& evt, UIView* sender) -> bool {
		static int changes = 0;
		changes++;

		std::string title = "Title Changed " + std::to_string(changes) + " times";
		window->SetTitle(title.c_str());

		return EVENT_HANDLED;
	});
	window->AddView(ButtonChangeTitle);

	window->StartWindowLoop();

    return 0;
}
