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

Shared<StackPanel> createLargePanel() {
    auto panel = MakeRef<StackPanel>();
    panel->marginLeft = 20;
    panel->marginTop = 20;
    panel->marginRight = 20;
    panel->orientation = Vertical;
    panel->backgroundColor = Color(0, 60, 20);

    auto label = MakeRef<Label>();
    label->position = { 100, 100 };
    label->color = Color::white;
    label->fontSize = 22;
    label->text = "StackPanel Demo";
    label->marginTop = 10;
    label->marginBottom = 10;
    label->marginLeft = 6;
    label->marginRight = 6;
    panel->addChild(label);

    for (auto i = 0; i < 3; ++i) {
        auto btn = MakeRef<Button>();
        btn->label->text = "Button " + std::to_string(i);
        btn->label->fontSize = 14;
        panel->addChild(btn);
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
    panel->addChild(label);

    auto button = MakeRef<Button>();
    button->label->text = "This is a very larged button text";
    button->label->fontSize = 24;
    button->fixedWidth = 460;
    button->fixedHeight = 100;
    button->marginLeft = 10;
    button->marginRight = 10;
    button->marginBottom = 10;
    panel->addChild(button);

    for (auto i = 3; i < 7; ++i) {
        auto btn = MakeRef<Button>();
        btn->label->text = "Button " + std::to_string(i);
        btn->label->fontSize = 14;
        panel->addChild(btn);
    }

    return panel;
}

int main() {
    AppManager::registerApplication("appId-041587");
    Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1060, 660, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(40, 40, 40);
    rootPanel->orientation = Horizontal;
    window->setRootWidget(rootPanel);

    auto demoPanel = MakeRef<StackPanel>();
    demoPanel->marginLeft = 20;
    demoPanel->marginTop = 20;
    demoPanel->marginRight = 20;
    demoPanel->orientation = Vertical;
    demoPanel->backgroundColor = Color(0, 60, 20);
    demoPanel->minWidth = 300;
    rootPanel->addChild(demoPanel);

    auto first = MakeRef<Button>();
    first->label->text = "First Button";
    first->label->fontSize = 18;
    first->marginLeft = 6;
    first->marginRight = 6;
    first->marginTop = 6;
    first->marginBottom = 6;
    demoPanel->addChild(first);

    auto second = MakeRef<Button>();
    second->label->text = "Second Button";
    second->label->fontSize = 18;
    second->marginLeft = 6;
    second->marginRight = 6;
    second->marginTop = 6;
    second->marginBottom = 6;
    demoPanel->addChild(second);

    rootPanel->addChild(createLargePanel());

    AppManager::startApplicationLoop();
    return 0;
}
