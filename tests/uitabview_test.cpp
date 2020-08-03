#include "Monochrome.h"
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1220, 760, "UITabView Test");
	window->SetBackgroundColor(Color(28, 21, 22, 1.0f));
	window->SetModernWindowButtonsColor(window->GetBackgroundColor());

	auto tabview = MakeRef<UITabView>(Frame(300, 80, 760, 600));
	tabview->AddTab("Element Properties")->layer.color = Color(80, 60, 10, 1);
	tabview->AddTab("Window Settings")->layer.color = Color(0, 80, 120, 1);
	tabview->AddTab("Config")->layer.color = Color(100, 40, 0, 1);
	tabview->AddTab("File 1")->layer.color = Color(0, 40, 20, 1);
	tabview->AddTab("File 2")->layer.color = Color(0, 20, 22, 1);
	tabview->StyleTabButtons([](Ref<UIButton>& tab) {
		tab->layer.color = Color(28, 28, 29, 1);
		tab->Label->color = Color::light_blue;
	});
	tabview->OpenTab("Window Settings");
	window->AddView(tabview);

	auto deleteButton = MakeRef<UIButton>();
	deleteButton->layer.frame.position += {100, 40};
	deleteButton->Label->Text = "Delete Tab";
	deleteButton->AddEventHandler<EventType::MouseButtonClicked>([tabview](Event& e, UIView* sender) -> bool {
		tabview->RemoveTab("Window Settings");
		return EVENT_HANDLED;
	});

	tabview->GetTab("Window Settings")->AddSubview(deleteButton);

	window->StartWindowLoop();
	return 0;
}