# Monochrome

Monochrome is a GUI library aimed at C++ developers to help make the process of GUI development much easier. Monochrome offers modern style window and UI elements.
The windowing system depends on Win32 API and graphics drawing is done by Direct2D and DirectWrite.

## Platform
Monochrome is currently only supported on Windows.

## Installation

After cloning the repo, open git bash in the master directory and execute the following commands:
```
mkdir build
cd build
cmake ..
```

The Visual Studio solution will be generated in the build directory.

## Sample Code

```cpp
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
		MessageBox(0, "Thank You for Using Monochrome!", "MC", NULL);
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
	return 0;
}
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[GPL-3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)
