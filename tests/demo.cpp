#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/Color.h>
#include <widgets/BaseWidget.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-032487");

    auto window = MakeRef<ClassicWindow>(1060, 660, "New Widget System Demo");
    window->setBackgroundColor(Color(18, 22, 28));

    auto testWidget = MakeRef<BaseWidget>();

    AppManager::startApplicationLoop();
    return 0;
}
