from Monochrome import *

def on_mouse_click(event, sender):
    print("Mouse Button Released: " + str(event.button))
    sender.layer.frame.position = (sender.layer.frame.position[0] + 2, sender.layer.frame.position[1])
    sender.layer.frame.size = (sender.layer.frame.size[0], sender.layer.frame.size[1] + 2)
    color = sender.layer.color
    sender.layer.color = (color[0] - 10, color[1], color[2] - 16)

window = UIWindow(700, 420, "MonoPy")

view = UIView()
view.add_mouse_clicked_event_handler(on_mouse_click)
window.add_view(view)

window.start_window_loop()
