from Monochrome import *

clicked_times = 0

def button_onclick():
    global button, clicked_times
    clicked_times += 1
    button.Label.Text = "Clicked " + str(clicked_times) + " Times"


if __name__ == '__main__':
    window = UIWindow()

    global button
    button = UIButton()
    button.layer.position = (200, 200)
    button.layer.size = (180, 50)
    button.layer.color = (90, 40, 40)
    button.Label.Text = "Click Me :D"
    button.AddMouseClickedEventHandler(button_onclick)
    window.AddView(button)

    window.StartWindowLoop()