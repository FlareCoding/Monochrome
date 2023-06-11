#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-splitter-demo");

    auto window = MakeRef<ClassicWindow>(1280, 580, "SplitterPanel Demo");

    auto rootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer("splitterpanel_example1.mcx");
    window->setRootWidget(rootPanel);

    AppManager::startApplicationLoop();
    return 0;
}
