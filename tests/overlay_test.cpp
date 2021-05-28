#include "Monochrome.h"
using namespace mc;

int main()
{
	auto window = UIWindow::CreateOverlayWindow("File Explorer", "Overlay Test");
	window->SetBackgroundColor(Color(0, 0, 0, 0));
	window->SetModernWindowButtonsColor(window->GetBackgroundColor());

	Ref<UIView> overlayMenu = MakeRef<UIView>();
	overlayMenu->layer.frame.position = { 100, 100 };
	overlayMenu->layer.frame.size = { 300, 300 };
	overlayMenu->layer.color = Color::navy;
	window->AddView(overlayMenu);

	Ref<UIButton> button = MakeRef<UIButton>();
	button->layer.frame = Frame({ 10, 10 }, { 280, 36 });
	button->layer.color = Color::gray;
	button->Label->layer.color = Color::black;
	button->Label->Text = "Press F2 to Show/Hide the Menu";
	overlayMenu->AddSubview(button);

	Ref<UIButton> button2 = MakeRef<UIButton>();
	button2->layer.frame = Frame({ 10, 56 }, { 280, 36 });
	button2->layer.color = Color::gray;
	button2->Label->layer.color = Color::black;
	button2->Label->Text = "Item 1";
	overlayMenu->AddSubview(button2);

	Ref<UIButton> button3 = MakeRef<UIButton>();
	button3->layer.frame = Frame({ 10, 102 }, { 280, 36 });
	button3->layer.color = Color::gray;
	button3->Label->layer.color = Color::black;
	button3->Label->Text = "Item 2";
	overlayMenu->AddSubview(button3);

	std::thread input_thread([&]() {
		while (true)
		{
			if (GetAsyncKeyState(VK_F2) & 1)
				overlayMenu->Visible = !overlayMenu->Visible;

			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	});
	input_thread.detach();

	window->StartWindowLoop();
	return 0;
}