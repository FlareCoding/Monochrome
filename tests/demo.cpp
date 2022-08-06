#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/Color.h>
#include <widgets/AllWidgets.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-041587");

    auto window = MakeRef<ClassicWindow>(1060, 660, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));

    auto rootPanel = MakeRef<StackPanel>();
    window->setRootWidget(rootPanel);

    auto fillButton = MakeRef<Button>();
    fillButton->text = "Fill";
    fillButton->marginLeft = 5;
    fillButton->marginRight = 5;
    fillButton->marginTop = 5;
    fillButton->marginBottom = 5;
    fillButton->horizontalAlignment = HAFill;
    rootPanel->addChild(fillButton);

    auto leftButton = MakeRef<Button>();
    leftButton->text = "Left";
    leftButton->marginLeft = 5;
    leftButton->marginRight = 5;
    leftButton->marginTop = 5;
    leftButton->marginBottom = 5;
    leftButton->horizontalAlignment = HALeft;
    rootPanel->addChild(leftButton);

    auto rightButton = MakeRef<Button>();
    rightButton->text = "Right";
    rightButton->marginLeft = 5;
    rightButton->marginRight = 5;
    rightButton->marginTop = 5;
    rightButton->marginBottom = 5;
    rightButton->horizontalAlignment = HARight;
    rootPanel->addChild(rightButton);

    auto centerButton = MakeRef<Button>();
    centerButton->text = "Center";
    centerButton->marginLeft = 5;
    centerButton->marginRight = 5;
    centerButton->marginTop = 5;
    centerButton->marginBottom = 5;
    centerButton->horizontalAlignment = HACenter;
    rootPanel->addChild(centerButton);

    AppManager::startApplicationLoop();
    return 0;
}
