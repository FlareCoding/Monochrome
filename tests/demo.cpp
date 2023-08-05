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

    auto backgroundImage = Image::loadFromWebUrl("https://img.freepik.com/free-photo/black-stones-tiled-floor_1194-6238.jpg?w=1060&t=st=1691277958~exp=1691278558~hmac=6b59888ae62120bee0ce146bda02cfb7cd8af31783b772133ab33ffc89c05a7c");
    backgroundImage->tilingEnabled = true;

    auto root = MakeRef<StackPanel>();
    root->setBackgroundImage(backgroundImage);
    window->setRootWidget(root);

    auto label = MakeRef<Label>("Label 1");
    root->addChild(label);

    AppManager::startApplicationLoop();
    return 0;
}
