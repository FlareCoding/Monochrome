#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-splitter-demo");

    auto mainWindow = MakeRef<ClassicWindow>(1280, 580, "SplitterPanel Demo");

    auto rootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer("splitterpanel_example1.mcx");
    mainWindow->setRootWidget(rootPanel);

    AppManager::startApplicationLoop();
    return 0;
}
