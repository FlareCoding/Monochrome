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

    auto window = MakeRef<ClassicWindow>(1180, 860, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));
    window->on("keyDown", [window](Shared<Event> e) {
        auto pressedChar = std::static_pointer_cast<KeyDownEvent>(e)->getChar();

        if (pressedChar == 'x') {
            Renderer::enableDebugBoundingBoxes = !Renderer::enableDebugBoundingBoxes;
            window->setShouldRedraw();
        }
    });
    
    AppManager::startApplicationLoop();
    return 0;
}
