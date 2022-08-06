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

    auto testWidget = MakeRef<Button>();
    window->setRootWidget(testWidget);

    AppManager::startApplicationLoop();
    return 0;
}
