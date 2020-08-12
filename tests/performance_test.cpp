#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <Monochrome.h>
using namespace mc;

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "Performance Test");
	window->SetBackgroundColor(Color(38, 38, 39, 1.0f));
	window->SetModernWindowButtonsColor(Color(38, 38, 39, 1.0f));

	static uint64_t ViewCount = 8;

	auto ViewCountLabel = MakeRef<UILabel>(Frame(360, 150, 260, 30));
	ViewCountLabel->Properties.Allignment = TextAlignment::TRAILING;
	ViewCountLabel->Text = "View Count: " + std::to_string(ViewCount);
	ViewCountLabel->color = Color::white;
	window->AddView(ViewCountLabel);

	auto EventProcessingTimeLabel = MakeRef<UILabel>(Frame(360, 180, 260, 30));
	EventProcessingTimeLabel->Properties.Allignment = TextAlignment::TRAILING;
	EventProcessingTimeLabel->Text = "Event Processing Time: " + std::to_string(0) + " seconds";
	EventProcessingTimeLabel->color = Color::white;
	window->AddView(EventProcessingTimeLabel);

	auto GraphcisRenderingTimeLabel = MakeRef<UILabel>(Frame(360, 210, 260, 30));
	GraphcisRenderingTimeLabel->Properties.Allignment = TextAlignment::TRAILING;
	GraphcisRenderingTimeLabel->Text = "Graphics Rendering Time: " + std::to_string(0) + " seconds";
	GraphcisRenderingTimeLabel->color = Color::white;
	window->AddView(GraphcisRenderingTimeLabel);

	auto PerformanceViewer = MakeRef<UIView>(Frame(-100, -100, 0, 0));
	PerformanceViewer->AddEventHandler<EventType::WindowUpdated>([&](Event& e, UIView* sender) -> bool {
		ViewCountLabel->Text			= "View Count: " + std::to_string(ViewCount);
		EventProcessingTimeLabel->Text	= "Event Processing Time: " + std::to_string(reinterpret_cast<WindowUpdatedEvent&>(e).EventProcessingTime) + "s";
		GraphcisRenderingTimeLabel->Text	= "Graphics Rendering Time: " + std::to_string(reinterpret_cast<WindowUpdatedEvent&>(e).GraphicsRenderingTime) + "s";
		return EVENT_HANDLED;
	});
	window->AddView(PerformanceViewer);

	auto ViewAdder100Button = MakeRef<UIButton>(Frame(40, 80, 220, 50));
	ViewAdder100Button->layer.color = Color::light_blue;
	ViewAdder100Button->Label->color = Color::blue;
	ViewAdder100Button->Label->Text = "Add 100 Views";
	ViewAdder100Button->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool {
		for (size_t i = 0; i < 100; i++)
		{
			auto TestView = MakeRef<UIView>(Frame(100 + (float)i * 2, 380 + (float)i, 40, 40));
			window->AddView(TestView);
			ViewCount++;
		}
		return EVENT_HANDLED;
	});
	window->AddView(ViewAdder100Button);

	auto ViewAdder1000Button = MakeRef<UIButton>(Frame(280, 80, 220, 50));
	ViewAdder1000Button->layer.color = Color::light_blue;
	ViewAdder1000Button->Label->color = Color::blue;
	ViewAdder1000Button->Label->Text = "Add 1000 Views";
	ViewAdder1000Button->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool {
		for (size_t i = 0; i < 1000; i++)
		{
			auto TestView = MakeRef<UIView>(Frame(400, 380, 40, 40));
			window->AddView(TestView);
			ViewCount++;
		}
		return EVENT_HANDLED;
	});
	window->AddView(ViewAdder1000Button);

	auto ViewAdder10KButton = MakeRef<UIButton>(Frame(520, 80, 220, 50));
	ViewAdder10KButton->layer.color = Color::light_blue;
	ViewAdder10KButton->Label->color = Color::blue;
	ViewAdder10KButton->Label->Text = "Add 10k Views";
	ViewAdder10KButton->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool {
		for (size_t i = 0; i < 10000; i++)
		{
			auto TestView = MakeRef<UIView>(Frame(600, 380, 40, 40));
			window->AddView(TestView);
			ViewCount++;
		}
		return EVENT_HANDLED;
	});
	window->AddView(ViewAdder10KButton);

	auto ViewAdder100KButton = MakeRef<UIButton>(Frame(760, 80, 220, 50));
	ViewAdder100KButton->layer.color = Color::light_blue;
	ViewAdder100KButton->Label->color = Color::blue;
	ViewAdder100KButton->Label->Text = "Add 100k Views";
	ViewAdder100KButton->AddEventHandler<EventType::MouseButtonClicked>([&](Event& e, UIView* sender) -> bool {
		for (size_t i = 0; i < 100000; i++)
		{
			auto TestView = MakeRef<UIView>(Frame(400, 440, 40, 40));
			window->AddView(TestView);
			ViewCount++;
		}
		return EVENT_HANDLED;
	});
	window->AddView(ViewAdder100KButton);

	window->StartWindowLoop();
}
