from Monochrome import *

def button_onclick(event, sender):
    global checkbox
    checkbox.checked = not checkbox.checked

def checkbox_value_changed(checked, sender):
    global slider
    if sender.checked:
        slider.value = 95
    else:
        slider.value = 32

def slider_value_changed(value, sender):
    print("Slider Value: " + str(value))

if __name__ == '__main__':
    global checkbox, slider

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
    checkbox.add_value_changed_event_handler(checkbox_value_changed)
    window.add_view(checkbox)

    slider = UISlider()
    slider.layer.frame.position = Point(160, 240)
    slider.layer.frame.size = Size(200, 20)
    slider.slider_knob_shape = 1
    slider.add_value_changed_event_handler(slider_value_changed)
    window.add_view(slider)

    window.start_window_loop()
    print("End of control flow reached, clean up code should go here")
