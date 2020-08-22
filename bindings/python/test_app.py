from Monochrome import *

clicked_times = 0

def button_onmousehoveron(e):
    print("MouseHoverOn")

def button_onmousehoveroff(e):
     print("MouseHoverOff")

def button_onmousescrolled(e):
     print("MouseScrolled")

def button_onmousemoved(e):
     print("MouseMoved")

def button_onpress(e):
    print("Mouse button " + str(e.button) + " pressed at location " + str(e.location))

def button_onclick(e):
    global button, clicked_times
    clicked_times += 1
    button.label.text = "Clicked " + str(clicked_times) + " Times"
    print("Mouse button " + str(e.button) + " released/clicked at location " + str(e.location))

def button_onkeypressed(e):
     print("KeyPressed: " + e.key)

def button_onkeyreleased(e):
    print("KeyReleased: " + e.key)

if __name__ == '__main__':
    window = UIWindow()

    global button
    button = UIButton()
    button.layer.position = (200, 200)
    button.layer.size = (180, 50)
    button.layer.color = (90, 40, 40)
    button.label.text = "Click Me :D"
    button.AddMousePressedEventHandler(button_onpress)
    button.AddMouseClickedEventHandler(button_onclick)
    button.AddMouseHoverOnEventHandler(button_onmousehoveron)
    button.AddMouseHoverOffEventHandler(button_onmousehoveroff)
    button.AddMouseMovedEventHandler(button_onmousemoved)
    button.AddMouseScrolledEventHandler(button_onmousescrolled)
    button.AddKeyPressedEventHandler(button_onkeypressed)
    button.AddKeyReleasedEventHandler(button_onkeyreleased)
    window.AddView(button)

    window.StartWindowLoop()