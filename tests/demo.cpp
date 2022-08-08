#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/Color.h>
#include <widgets/AllWidgets.h>
using namespace mc;

#include <rendering/Renderer.h>

int main() {
    AppManager::registerApplication("appId-041587");
    Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1060, 660, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(40, 40, 40);
    window->setRootWidget(rootPanel);

    auto demoPanel = MakeRef<StackPanel>();
    demoPanel->marginLeft = 50;
    demoPanel->marginTop = 50;
    demoPanel->orientation = Vertical;
    demoPanel->backgroundColor = Color(0, 60, 20);
    rootPanel->addChild(demoPanel);

    auto first = MakeRef<Button>();
    first->label->text = "First Button";
    first->marginLeft = 6;
    first->marginTop = 6;
    first->marginBottom = 6;
    demoPanel->addChild(first);

    auto second = MakeRef<Button>();
    second->label->text = "Second Button";
    second->marginLeft = 6;
    second->marginTop = 6;
    second->marginRight = 6;
    second->marginBottom = 6;
    demoPanel->addChild(second);

    /*auto label = MakeRef<Label>();
    label->position = { 100, 100 };
    label->color = Color::white;
    label->fontSize = 22;
    label->text = "StackPanel Demo";
    label->marginTop = 10;
    label->marginBottom = 10;
    label->marginLeft = 6;
    label->marginRight = 6;
    demoPanel->addChild(label);

    for (auto i = 0; i < 3; ++i) {
        auto btn = MakeRef<Button>();
        btn->label->text = "Button " + std::to_string(i);
        btn->label->fontSize = 14;
        btn->marginLeft = 10;
        demoPanel->addChild(btn);
    }

    label = MakeRef<Label>();
    label->position = { 100, 100 };
    label->color = Color::white;
    label->fontSize = 16;
    label->text = "Smaller label";
    label->marginTop = 10;
    label->marginBottom = 10;
    label->marginLeft = 6;
    label->marginRight = 6;
    demoPanel->addChild(label);

    auto button = MakeRef<Button>();
    button->label->text = "This is a very long button text";
    button->label->fontSize = 24;
    button->fixedWidth = 460;
    button->fixedHeight = 100;
    demoPanel->addChild(button);

    for (auto i = 3; i < 7; ++i) {
        auto btn = MakeRef<Button>();
        btn->label->text = "Button " + std::to_string(i);
        btn->label->fontSize = 14;
        demoPanel->addChild(btn);
    }*/

    AppManager::startApplicationLoop();
    return 0;
}
