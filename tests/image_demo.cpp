#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-image-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "Image Demo");

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto topImage = Image::loadFromWebUrl("https://img.freepik.com/free-photo/breathtaking-shot-mountainous-landscape-with-ocean-captured-reine-norway_181624-49195.jpg?w=1060&t=st=1685803686~exp=1685804286~hmac=588175b0e40be03bc08d84a7f8c2c9158b545984d3cf0d60a7ec886c4988f7a3");
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
