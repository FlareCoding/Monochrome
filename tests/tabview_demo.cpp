#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-tabview-demo");

    auto window = mcx::McxEngine::parseWindowFile("tabview_demo.mcx");

    auto splitterPanel = getWidgetById<SplitterPanel>("splitterPanel");

    for (int i = 0; i < 3; i++) {
        auto lbl = MakeRef<Label>();
        lbl->marginLeft = 80;
        lbl->marginTop = 10;
        lbl->text = "You opened the third tab :D";
        splitterPanel->addChild(lbl);
    }

    AppManager::startApplicationLoop();
    return 0;
}
