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

int main() {
    AppManager::registerApplication("appId-041587");
    //Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1180, 860, "Monochrome Demo");
    BaseWidget::setUniversalBackgroundColor(window->getBackgroundColor());

    auto root = MakeRef<StackPanel>();
    window->setRootWidget(root);

    auto label = MakeRef<Label>("Monochrome Widgets Demo");
    label->marginTop = 6;
    label->marginBottom = 12;
    root->addChild(label);

    auto buttonsGroupExpander = MakeRef<Expander>("Buttons");
    buttonsGroupExpander->horizontalAlignment = HACenter;
    buttonsGroupExpander->header->label->fontSize = 12;
    root->addChild(buttonsGroupExpander);

    auto defaultButton = MakeRef<Button>("Click Me");
    defaultButton->marginRight = 6;

    auto disabledButton = MakeRef<Button>("Disabled");
    disabledButton->enabled = false;

    defaultButton->on("clicked", [disabledButton](Shared<Event> e) {
        disabledButton->enabled = !disabledButton->enabled.get();
    });

    auto buttonsGroup = MakeRef<StackPanel>();
    buttonsGroup->marginTop = 6;
    buttonsGroup->marginBottom = 6;
    buttonsGroup->orientation = Horizontal;
    buttonsGroup->addChild(defaultButton);
    buttonsGroup->addChild(disabledButton);

    auto buttonsGroupContainer = MakeRef<StackPanel>();
    buttonsGroupContainer->backgroundColor = Color(30, 30, 31);
    buttonsGroupContainer->addChild(buttonsGroup);
    buttonsGroupExpander->setContent(buttonsGroupContainer);

    AppManager::startApplicationLoop();
    return 0;
}
