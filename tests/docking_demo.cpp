#include <random>
#include "Monochrome.h"
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1220, 760, "Docking Demo");
	window->SetBackgroundColor(Color(26, 25, 24));
	window->SetModernWindowButtonsColor(Color(26, 25, 24));

	auto dockPanel = MakeRef<UIDockPanel>(Frame(140, 140, 600, 450));
	dockPanel->SetAnchoredToWindow(true);
	window->AddView(dockPanel);

	auto leftPanel = MakeRef<UIView>(Frame(0, 0, 340, 0));
	leftPanel->layer.color = Color::green;
	leftPanel->anchor = Anchor::Left;
	dockPanel->AddSubview(leftPanel);

	auto rightPanel = MakeRef<UIView>(Frame(0, 0, 340, 0));
	rightPanel->layer.color = Color::gray;
	rightPanel->anchor = Anchor::Right;
	dockPanel->AddSubview(rightPanel);

	auto centerPanel = MakeRef<UIView>();
	centerPanel->layer.color = Color::navy;
	centerPanel->anchor = Anchor::Center;
	dockPanel->AddSubview(centerPanel);

	auto testCenterButton = MakeRef<UIButton>();
	centerPanel->AddSubview(testCenterButton);

	auto bottomButton = MakeRef<UIButton>(Frame(0, 0, 0, 80));
	bottomButton->Label->Text = "Sample Button";
	bottomButton->anchor = Anchor::Bottom;
	bottomButton->Label->color = Color::blue;
	bottomButton->layer.color = Color::purple;
	dockPanel->AddSubview(bottomButton);

    window->StartWindowLoop();
	return 0;
}
