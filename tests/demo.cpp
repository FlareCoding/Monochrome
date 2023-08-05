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
    Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1180, 860, "Monochrome Demo");
    BaseWidget::setUniversalBackgroundColor(window->getBackgroundColor());

    auto root = MakeRef<StackPanel>();
    window->setRootWidget(root);

    auto label = MakeRef<Label>("Label 1");
    label->marginLeft = 10;
    label->marginRight = 10;
    root->addChild(label);

    auto label2 = MakeRef<Label>("Label 2");
    label2->marginLeft = 10;
    label2->marginRight = 10;
    root->addChild(label2);

    auto label3 = MakeRef<Label>("Label 3");
    label3->marginLeft = 10;
    label3->marginRight = 10;
    root->addChild(label3);

    label->layoutWeight = 2;
    label3->layoutWeight = 1;
    
    AppManager::startApplicationLoop();
    return 0;
}
