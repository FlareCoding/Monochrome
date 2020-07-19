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
	window->GetCloseButtonRef()->layer.color = Color(28, 21, 31, 1.0f);

	Ref<UIButton> button = MakeRef<UIButton>();
	button->layer.frame = Frame({ 340, 120 }, { 200, 36 });
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

	window->StartWindowLoop();

	MessageBoxA(0, "End of control flow reached\r\nClean up code should go here", "Ready to Return", 0);

	return 0;
}
