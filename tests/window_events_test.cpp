#include <Monochrome.h>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "Window Events Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));

	const auto evt_handler = [](Event& e, UIView* sender) -> bool {
		printf("%s\n", e.ToString().c_str());
		return EVENT_HANDLED;
	};

	auto view = MakeRef<UIView>(Frame(200, 200, 380, 300));
    view->layer.color = Color::light_blue;
	view->AddEventHandler<EventType::WindowResized>(evt_handler);
	view->AddEventHandler<EventType::WindowGainedFocus>(evt_handler);
	view->AddEventHandler<EventType::WindowLostFocus>(evt_handler);
	view->AddEventHandler<EventType::WindowClosed>(evt_handler);
    window->AddView(view);

	window->StartWindowLoop();
}
