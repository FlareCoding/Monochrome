from Monochrome import *

def style_tab_button(tab):
    tab.layer.color = Color(28, 28, 29)
    tab.label.color = Color(0, 80, 200)

def deleteButton_onclick(event, sender):
    global tabview
    tabview.remove_tab("Window Settings")

window = UIWindow(1220, 760, "UITabView Test")
window.set_background_color(Color(28, 21, 22))
window.set_modern_window_buttons_color(Color(28, 21, 22))

tabview = UITabView()
tabview.layer.frame.position = Point(300, 80)
tabview.layer.frame.size = Size(760, 600)
tabview.add_tab("Element Properties").layer.color = Color(80, 60, 10)
tabview.add_tab("Window Settings").layer.color = Color(0, 80, 120)
tabview.add_tab("Config").layer.color = Color(100, 40, 0)
tabview.add_tab("File 1").layer.color = Color(0, 40, 20)
tabview.add_tab("File 2").layer.color = Color(0, 20, 22)
tabview.style_tab_buttons(style_tab_button)
tabview.open_tab("Window Settings")
window.add_view(tabview)

deleteButton = UIButton()
deleteButton.layer.frame.position = Point(160, 80)
deleteButton.label.text = "Delete Tab"
deleteButton.add_mouse_clicked_event_handler(deleteButton_onclick)

tabview.get_tab("Window Settings").add_subview(deleteButton)

window.start_window_loop()
