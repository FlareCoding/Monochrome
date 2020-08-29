#include "Monochrome.h"
using namespace mc;

Ref<UICheckbox> checkbox;
Ref<UISlider> slider;
Ref<UITextArea> textArea;
Ref<UIProgressBar> progressBar;

void Checkbox_ValueChanged(bool checked, UICheckbox* sender)
{
	slider->Value = (checked) ? 95.0f : 32.0f;
}

void Slider_ValueChanged(float value, UISlider* sender)
{
	printf("Slider Value: %f\n", value);
}

void ChangeProgressBarValue()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		progressBar->Value += 1;

		if (progressBar->Value > 100)
			progressBar->Value = 0;
	}
}

int main()
{
	auto window = UIWindow::Create(WindowStyle::Modern, 1220, 760, "Demo App");
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
	textbox->textProperties.FontSize = 16;
	textbox->AddEventHandler<EventType::KeyPressed>([](Event& e, UIView* sender) -> bool {
		if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			printf("Text entered from Textbox 1!\n");

		return EVENT_HANDLED;
	});
	window->AddView(textbox);

	Ref<UITextbox> textbox2 = MakeRef<UITextbox>();
	textbox2->layer.frame = Frame(Position{ 220, 400 }, Size{ 260, 34 });
	textbox2->textProperties.FontSize = 16;
	textbox2->Placeholder = "Enter Username";
	textbox2->AddEventHandler<EventType::KeyPressed>([](Event& e, UIView* sender) -> bool {
		if (((KeyPressedEvent&)e).keycode == KeyCode::KEY_RETURN)
			printf("Text entered from Textbox 2!\n");

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

	Ref<UICombobox> combobox = MakeRef<UICombobox>();
	combobox->layer.frame = Frame(Position{ 960, 100 }, Size{ 180, 100 });
	std::vector<std::string> items = { "Red", "Green", "Blue", "Purple", "Cyan", "Pink", "Brown" };
	combobox->SetItems(items);
	combobox->SetItemBackgroundColor(Color(58, 58, 59, 1));
	combobox->SetItemTextColor(Color::white);
	combobox->layer.color = Color(62, 62, 63, 1);
	combobox->SetDropdownArrowColor(Color::white);
	combobox->SetSelectedItemColor(Color::white);
	combobox->AddItemChangedEventHandler([](size_t index, UICombobox* sender) {
		printf("Selected \"%s\" at index %zu\n", sender->GetItem(index).c_str(), index);
	});
	window->AddView(combobox);

	textArea = MakeRef<UITextArea>();
	textArea->layer.frame = Frame(Position{ 140, 470 }, Size{ 320, 220 });
	textArea->RightMargins = 4.0f;
	textArea->LeftMargins = 4.0f;
	window->AddView(textArea);

	progressBar = MakeRef<UIProgressBar>();
	progressBar->layer.frame = Frame(Position { 600, 660 }, Size { 240, 12 });
	progressBar->Value = 0;
	window->AddView(progressBar);
	std::thread progressBarThread(ChangeProgressBarValue);
	progressBarThread.detach();

	window->StartWindowLoop();

	printf("End of control flow reached, clean up code should go here\n");
	return 0;
}
