#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-mcstudio");

    auto window = MakeRef<ClassicWindow>(1460, 920, "Monochrome Studio");

    AppManager::startApplicationLoop();
    return 0;
}
