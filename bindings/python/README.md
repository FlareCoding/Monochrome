## Installation/Usage

- To install the Monochrome python module execute the following:
    1) Open terminal/cmd with admin privileges in this directory that contains the 'src' folder.
    2) Run: ```python3 setup.py install```


## Sample Code

```py
from Monochrome import *

def button_onclick(event, sender):
    print("Button Clicked!")

if __name__ == '__main__':
    window = UIWindow(1220, 760, "Demo App")
    window.set_background_color(Color(28, 21, 31))
    window.set_modern_window_buttons_color(Color(28, 21, 31))

    button = UIButton()
    button.layer.frame.position = Point(280, 120)
    button.layer.frame.size = Size(200, 36)
    button.label.text = "Click Me"
    button.add_mouse_clicked_event_handler(button_onclick)
    window.add_view(button)

    checkbox = UICheckbox()
    checkbox.layer.frame.position = Point(160, 160)
    checkbox.layer.frame.size = Size(300, 40)
    window.add_view(checkbox)

    window.StartWindowLoop()
```