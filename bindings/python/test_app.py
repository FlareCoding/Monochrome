from Monochrome import *

def on_mouse_click(event, sender):
    print("Mouse Button Released: " + str(event.button))
    sender.layer.frame.position.x += 2
    sender.layer.frame.size.y += 2
    sender.layer.color.r -= 5
    sender.layer.color.g -= 10

window = UIWindow(700, 420, "MonoPy")

button = UIButton()
button.label.text = "Cool Button"
button.label.properties.font = "Impact"
button.label.color.b -= 120
button.on_mouse_press_color = Color(40, 0, 0)
button.hover_on_color = Color(70, 0, 0)
button.add_mouse_clicked_event_handler(on_mouse_click)
window.add_view(button)

window.start_window_loop()
