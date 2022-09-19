#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
#include <utils/FileWatcher.h>
using namespace mc;

const std::string rootMcxFilepath = "../tests/mcxapp_demo.mcx";

int main() {
    AppManager::registerApplication("appId-mcxapp-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "McxApp Demo");

    auto root = mcx::McxEngine::parseUserWidgetFileAsContainer(rootMcxFilepath);
    window->setRootWidget(root);

    // utils::FileWatcher watcher;
    // watcher.watchFile(rootMcxFilepath);
    // watcher.on("fileModified", [window](Shared<Event> e) {
    //     AppManager::executeOnUiThread([window]() {
    //         auto rootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer(rootMcxFilepath);
    //         window->setRootWidget(rootPanel);
    //     });
    // });

    auto progressBar = getWidgetById<ProgressBar>("mainProgressBar");
    progressBar->progressColor = Color(240, 170, 0);
    progressBar->value = 75;

    AppManager::startApplicationLoop();
    return 0;
}
