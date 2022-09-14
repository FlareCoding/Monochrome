#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

int main() {
    AppManager::registerApplication("appId-image-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "DockPanel Demo");

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto topImage = Image::loadFromWebUrl("https://images.unsplash.com/photo-1663106516321-2575f0dfd562?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=3200&q=80");
    topImage->horizontalAlignment = HACenter;
    topImage->fixedWidth = 500;
    topImage->fixedHeight = 400;
    rootPanel->addChild(topImage);

    AppManager::startApplicationLoop();
    return 0;
}
