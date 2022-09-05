#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
using namespace mc;

auto makeButton() {
    auto button = MakeRef<Button>();
    button->cornerRadius = 0;
    button->borderThickness = 3;
    button->borderColor = Color::randomColor();

    return button;
}

int main() {
    AppManager::registerApplication("appId-monodev-devtools");

    auto window = MakeRef<ClassicWindow>(860, 640, "DockPanel Demo");

    auto rootPanel = MakeRef<DockPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto topButton = makeButton();
    topButton->label->text = "Top Button";
    topButton->fixedHeight = 100;
    topButton->dockAnchor = DockAnchor::Top;

    auto leftButton = makeButton();
    leftButton->label->text = "Left Button";
    leftButton->dockAnchor = DockAnchor::Left;

    auto rightButton = makeButton();
    rightButton->label->text = "Right Button";
    rightButton->dockAnchor = DockAnchor::Right;

    auto bottomButton = makeButton();
    bottomButton->label->text = "Bottom Button";
    bottomButton->fixedHeight = 80;
    bottomButton->dockAnchor = DockAnchor::Bottom;

    auto fillButton = makeButton();
    fillButton->label->text = "Fill Button";
    fillButton->dockAnchor = DockAnchor::Fill;

    rootPanel->addChild(leftButton);
    rootPanel->addChild(topButton);
    rootPanel->addChild(rightButton);
    rootPanel->addChild(bottomButton);
    rootPanel->addChild(fillButton);

    AppManager::startApplicationLoop();
    return 0;
}
