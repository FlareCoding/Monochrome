#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-mcstudio");

    registerCustomColor("mcstudioBgColorPrimary", Color(34, 33, 38));

    auto window = mcx::McxEngine::parseWindowFile("mcx/main.mcx");

    AppManager::startApplicationLoop();
    return 0;
}
