from Monochrome import *

window = UIWindow(1220, 560, "UIImage Test")
window.set_background_color(Color(26, 21, 22))
window.set_modern_window_buttons_color(window.get_background_color())

webimage1 = UIImage()
webimage1.layer.frame.position = Point(100, 100)
webimage1.layer.frame.size = Size(500, 300)
webimage1.load_web_image("http://wallpapers-images.ru/1920x1080/nature/wallpapers/wallpapers-nature-1.jpg")
if not webimage1.is_image_loaded():
    print("Image 1 was not successfully loaded from the web")

window.add_view(webimage1)

webimage2 = UIImage()
webimage2.layer.frame.position = Point(640, 100)
webimage2.layer.frame.size = Size(500, 300)
webimage2.load_web_image("http://wallpapers-images.ru/1920x1080/nature/wallpapers/wallpapers-nature-3.jpg")
if not webimage2.is_image_loaded():
    print("Image 2 was not successfully loaded from the web")

window.add_view(webimage2)

window.start_window_loop()