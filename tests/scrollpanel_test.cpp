#include "Monochrome.h"
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1200, 760, "Docking Demo");
	window->SetBackgroundColor(Color(30, 25, 24));
	window->SetModernWindowButtonsColor(Color(30, 25, 24));

	auto scrollPanel = MakeRef<UIScrollPanel>(Frame(300, 100, 600, 500));
	scrollPanel->layer.color = Color::gray;
	scrollPanel->ScrollbarColor = Color::white;
	scrollPanel->ContentView->layer.frame.size = Size{ 800, 800 };
	window->AddView(scrollPanel);

	int n = 1;
	for (int c = 0; c < 3; c++, n++)
	{
		for (int i = 0; i < 10; i++, n++)
		{
			Ref<UIButton> btn = MakeRef<UIButton>();
			btn->layer.frame = Frame({ 30 + (float)c * 220, 30 + (float)i * 70 }, { 200, 36 });
			btn->Label->Text = "Button " + std::to_string(n);
			scrollPanel->AddChild(btn);
		}
	}

    window->StartWindowLoop();
	return 0;
}
