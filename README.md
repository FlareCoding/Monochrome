# Monochrome

[![Build Status](https://travis-ci.org/FlareCoding/Monochrome.svg?branch=master)](https://travis-ci.org/FlareCoding/Monochrome)

Monochrome is an open source cross-platform GUI library and framework aimed primarily at C++ developers to help make the process of desktop GUI development much easier. Monochrome offers modern style window and UI elements. The windowing and graphics systems depend on platform-dependent native APIs to operate at highest efficiency.

## Platform

| Linux | Windows | MacOS |
|:--------:| :-: | :-: |
| ✓    | ✓ | ✓

## Installation

After cloning the repo, open git bash in the master directory and execute the following commands:
```
mkdir build
cd build
cmake ..
```

The Visual Studio solution will be generated in the build directory.

## Dependencies

Installing additional dependencies is only required on **Linux**.  
The following dependencies are required:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-- **X11** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Window Management)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-- **Cairo** &nbsp;&nbsp;&nbsp;&nbsp;(General Graphics)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-- **Pango** &nbsp;&nbsp;(Text Rendering)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-- **Glib** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(Required by Pango)  

***System commands for installation:***  
**X11:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;```sudo apt-get install -y libx11-dev```  
**Cairo:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;```sudo apt-get install -y libcairo2-dev```  
**Pango:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;```sudo apt-get install -y libsdl-pango-dev```  
**Glib:**&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;```sudo apt-get install -y libglib2.0-dev```  

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
[LGPL-2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)
