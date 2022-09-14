#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-image-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "DockPanel Demo");

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto topImage = Image::loadFromWebUrl("https://images.unsplash.com/photo-1663106516321-2575f0dfd562?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=3200&q=80");
    topImage->horizontalAlignment = HACenter;
    topImage->fixedWidth = 500;
    topImage->fixedHeight = 400;
    topImage->marginTop = 10;
    topImage->marginBottom = 10;
    rootPanel->addChild(topImage);

    auto iconButton = MakeRef<Button>();
    iconButton->horizontalAlignment = HACenter;
    iconButton->marginTop = 10;
    iconButton->marginBottom = 10;
    iconButton->setImage(Image::loadFromWebUrl("https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678077-computer-1024.png"));
    iconButton->imagePlacement = Icon;
    iconButton->label->text = "Computer Button";
    rootPanel->addChild(iconButton);

    auto imageButton = MakeRef<Button>();
    imageButton->horizontalAlignment = HACenter;
    imageButton->marginTop = 10;
    imageButton->marginBottom = 10;
    imageButton->setImage(Image::loadFromWebUrl("https://cdn0.iconfinder.com/data/icons/seo-web-4-1/128/Vigor_Movie-Video-Player-youtube-256.png"));
    imageButton->imagePlacement = Cover;
    imageButton->label->text = "";
    imageButton->fixedWidth = 90;
    imageButton->fixedHeight = 90;
    imageButton->borderColor = Color::transparent;
    rootPanel->addChild(imageButton);

    AppManager::startApplicationLoop();
    return 0;
}
