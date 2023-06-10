#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

Shared<BaseContainerWidget> secondRootPanel;

Shared<ClassicWindow> createSecondWindow(Shared<ClassicWindow> mainWindow) {
    auto window = MakeRef<ClassicWindow>(580, 580, "Test Controller");

    auto root = MakeRef<StackPanel>();
    root->backgroundColor = Color::darkGray;
    window->setRootWidget(root);

    auto btn = MakeRef<Button>();
    root->addChild(btn);
    btn->on("clicked", [mainWindow](Shared<Event> e) {
        secondRootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer("splitterpanel_example2.mcx");
        mainWindow->setRootWidget(secondRootPanel);
        std::cout << "threadID: " << std::this_thread::get_id() << "\n";
    });

    return window;
}

int main() {
    AppManager::registerApplication("appId-splitter-demo");

    auto mainWindow = MakeRef<ClassicWindow>(1280, 580, "SplitterPanel Demo");

    auto rootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer("splitterpanel_example1.mcx");
    mainWindow->setRootWidget(rootPanel);

    //secondRootPanel = mcx::McxEngine::parseUserWidgetFileAsContainer("splitterpanel_example2.mcx");
    std::cout << "main threadID: " << std::this_thread::get_id() << "\n";

    auto secondWindow = createSecondWindow(mainWindow);

    AppManager::startApplicationLoop();
    return 0;
}
