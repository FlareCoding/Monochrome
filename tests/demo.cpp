#include "Monochrome.h"
using namespace mc;

Ref<UICheckbox> checkbox;
Ref<UISlider> slider;

void Checkbox_ValueChanged(bool checked, UICheckbox* sender)
{
	slider->Value = (checked) ? 95.0f : 32.0f;
}

void Slider_ValueChanged(float value, UISlider* sender)
{
	printf("Slider Value: %f\n", value);
}

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1000, 670, "Demo App");
	window->SetBackgroundColor(Color(28, 21, 31, 1.0f));
	window->SetModernWindowButtonsColor(Color(28, 21, 31, 1.0f));

	Ref<UIButton> button = MakeRef<UIButton>();
	button->layer.frame = Frame({ 280, 120 }, { 200, 36 });
	button->Label->Text = "Click Me";
	button->AddEventHandler<EventType::MouseButtonClicked>([](Event& evt, UIView* sender) -> bool {
		checkbox->Checked = !checkbox->Checked;
		return EVENT_HANDLED; 
	});
	window->AddView(button);

	checkbox = MakeRef<UICheckbox>();
	checkbox->layer.frame = Frame(Position{ 160, 160 }, Size{ 300, 40 });
	checkbox->AddValueChangedEventHandler(Checkbox_ValueChanged);
	window->AddView(checkbox);

	slider = MakeRef<UISlider>();
	slider->layer.frame = Frame({ 160, 240 }, { 200, 20 });
	slider->SliderKnobShape = Shape::Circle;
	slider->AddValueChangedEventHandler(Slider_ValueChanged);
	window->AddView(slider);

	Ref<UITextbox> textbox = MakeRef<UITextbox>();
	textbox->layer.frame = Frame(Position{ 220, 340 }, Size{ 260, 34 });
	textbox->TextProperties.FontSize = 16;
	textbox->AddEventHandler<EventType::KeyPressed>([](Event& e, UIView* sender) -> bool {
		if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			MessageBoxA(0, "Text Entered!", "Textbox 1", 0);

		return EVENT_HANDLED;
	});
	window->AddView(textbox);

	Ref<UITextbox> textbox2 = MakeRef<UITextbox>();
	textbox2->layer.frame = Frame(Position{ 220, 400 }, Size{ 260, 34 });
	textbox2->TextProperties.FontSize = 16;
	textbox2->Placeholder = "Enter Username";
	textbox2->AddEventHandler<EventType::KeyPressed>([](Event& e, UIView* sender) -> bool {
		if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			MessageBoxA(0, "Username Entered!", "Textbox 2", 0);

		return EVENT_HANDLED;
	});
	window->AddView(textbox2);

	Ref<UIScrollPanel> scrollPanel = MakeRef<UIScrollPanel>();
	scrollPanel->layer.frame = Frame(Position{ 560, 100 }, Size{ 340, 500 });
	scrollPanel->layer.color = Color(200, 200, 200, 1.0f);
	scrollPanel->ContentView->layer.frame.size.height = 800;
	scrollPanel->ContentView->layer.color = Color::gray;
	window->AddView(scrollPanel);

	for (int i = 0; i < 10; i++)
	{
		Ref<UIButton> btn = MakeRef<UIButton>();
		btn->layer.frame = Frame({ 70, 30 + (float)i * 70 }, { 200, 36 });
		btn->Label->Text = "Button " + std::to_string(i + 1);
		scrollPanel->AddChild(btn);
	}

	window->StartWindowLoop();

	MessageBoxA(0, "End of control flow reached\r\nClean up code should go here", "Ready to Return", 0);

	return 0;
}
