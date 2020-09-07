from Monochrome import *

def button_onclick(event, sender):
    global checkbox
    checkbox.checked = not ccheckbox.checked

def checkbox_value_changed(checked, sender):
    global slider
    if sender.checked:
        slider.value = 95
    else:
        slider.value = 32

def slider_value_changed(value, sender):
    print("Slider Value: " + str(value))

def textbox1_textentered(event, sender):
    if event.keycode == 52:
        print("Text entered from Textbox 1!")

def textbox2_textentered(event, sender):
    if event.keycode == 52:
        print("Text entered from Textbox 2!")

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

    textbox1 = UITextbox()
    textbox1.layer.frame.position = Point(220, 340)
    textbox1.layer.frame.size = Size(260, 34)
    textbox1.text_properties.font_size = 16
    textbox1.add_key_pressed_event_handler(textbox1_textentered)
    window.add_view(textbox1)

    textbox2 = UITextbox()
    textbox2.layer.frame.position = Point(220, 400)
    textbox2.layer.frame.size = Size(260, 34)
    textbox2.text_properties.font_size = 16
    textbox2.add_key_pressed_event_handler(textbox2_textentered)
    window.add_view(textbox2)

    scrollPanel = UIScrollPanel()
    scrollPanel.layer.frame.position = Point(560, 100)
    scrollPanel.layer.frame.size = Size(340, 500)
    scrollPanel.layer.color = Color(200, 200, 200)
    scrollPanel.content_view.layer.frame.size.height = 800
    scrollPanel.content_view.layer.color = Color(120, 120, 120)
    window.add_view(scrollPanel)

    for i in range(0, 10):
        btn = UIButton()
        btn.layer.frame.position = Point(70, 30 + i * 70)
        btn.layer.frame.size = Size(200, 36)
        btn.label.text = "Button " + str(i + 1)
        scrollPanel.add_child(btn)

    window.start_window_loop()
    print("End of control flow reached, clean up code should go here")
