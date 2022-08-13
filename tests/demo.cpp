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
#include <events/KeyboardEvents.h>

Shared<StackPanel> createLargePanel() {
    auto panel = MakeRef<StackPanel>();
    panel->marginLeft = 20;
    panel->marginRight = 20;
    panel->marginTop = 20;
    panel->marginBottom = 20;
    panel->orientation = Vertical;
    panel->verticalAlignment = VACenter;
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
    label->horizontalAlignment = HAFill;
    panel->addChild(label);

    for (auto i = 0; i < 3; ++i) {
        auto btn = MakeRef<Button>();
        btn->label->text = "Button " + std::to_string(i);
        btn->label->fontSize = 14;
        btn->marginLeft = 10;
        btn->marginRight = 10;
        panel->addChild(btn);
    }

    auto smallerlabel = MakeRef<Label>();
    smallerlabel->position = { 100, 100 };
    smallerlabel->color = Color::white;
    smallerlabel->fontSize = 18;
    smallerlabel->text = "Smaller label";
    smallerlabel->marginTop = 10;
    smallerlabel->marginBottom = 10;
    smallerlabel->marginLeft = 6;
    smallerlabel->marginRight = 6;
    smallerlabel->horizontalAlignment = HACenter;
    panel->addChild(smallerlabel);

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
        btn->marginLeft = 10;
        btn->marginRight = 10;
        btn->horizontalAlignment = HARight;
        panel->addChild(btn);
    }

    return panel;
}

int main() {
    AppManager::registerApplication("appId-041587");
    Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1060, 660, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));
    window->on("keyDown", [window](Shared<Event> e) {
        auto pressedChar = std::static_pointer_cast<KeyDownEvent>(e)->getChar();

        if (pressedChar == 'x') {
            Renderer::enableDebugBoundingBoxes = !Renderer::enableDebugBoundingBoxes;
            window->setShouldRedraw();
        }
    });

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(40, 40, 40);
    rootPanel->orientation = Vertical;
    window->setRootWidget(rootPanel);

    auto centerPanel = MakeRef<StackPanel>();
    centerPanel->backgroundColor = Color(40, 40, 180);
    centerPanel->orientation = Horizontal;
    centerPanel->horizontalAlignment = HACenter;
    centerPanel->marginTop = 80;
    rootPanel->addChild(centerPanel);

    auto demoPanel = MakeRef<StackPanel>();
    demoPanel->marginLeft = 60;
    demoPanel->marginRight = 20;
    demoPanel->marginTop = 20;
    demoPanel->marginBottom = 20;
    demoPanel->orientation = Vertical;
    demoPanel->backgroundColor = Color(0, 60, 20);
    demoPanel->minWidth = 300;
    demoPanel->verticalAlignment = VAFill;
    centerPanel->addChild(demoPanel);

    auto hintLabel = MakeRef<Label>();
    hintLabel->text = "Press X to toggle debug boxes";
    hintLabel->fontSize = 26;
    hintLabel->marginTop = 10;
    hintLabel->marginBottom = 10;
    hintLabel->marginLeft = 10;
    hintLabel->marginRight = 10;
    demoPanel->addChild(hintLabel);

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
    second->marginBottom = 16;
    demoPanel->addChild(second);

    auto checkbox = MakeRef<Checkbox>();
    checkbox->marginLeft = 6;
    demoPanel->addChild(checkbox);

    auto progressBar = MakeRef<ProgressBar>();
    progressBar->marginTop = 16;
    progressBar->marginLeft = 6;
    demoPanel->addChild(progressBar);

    auto entry = MakeRef<Entry>();
    entry->marginTop = 16;
    entry->marginLeft = 6;
    demoPanel->addChild(entry);

    centerPanel->addChild(createLargePanel());

    AppManager::startApplicationLoop();
    return 0;
}
