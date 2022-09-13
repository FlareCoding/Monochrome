#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-mcxapp-demo");

    auto window = mcx::McxEngine::parseWindowFile("../tests/mcxapp_demo.mcx");

    auto progressBar = getWidgetById<ProgressBar>("mainProgressBar");
    progressBar->progressColor = Color(240, 170, 0);
    progressBar->value = 75;

    AppManager::startApplicationLoop();
    return 0;
}
