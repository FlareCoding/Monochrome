#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

Shared<BaseWidget> createTab1() {
    auto tab = MakeRef<StackPanel>();
    tab->backgroundColor = Color(0, 50, 20);

    auto btn = MakeRef<Button>();
    btn->label->text = "First tab's button :)";
    btn->marginLeft = 10;
    btn->marginRight = 10;
    btn->marginTop = 10;
    btn->marginBottom = 10;
    tab->addChild(btn);

    return tab;
}

Shared<BaseWidget> createTab2() {
    auto tab = MakeRef<StackPanel>();
    tab->backgroundColor = Color(100, 50, 20);

    auto lbl = MakeRef<Label>();
    lbl->marginLeft = 80;
    lbl->marginRight = 10;
    lbl->marginTop = 10;
    lbl->marginBottom = 10;
    lbl->text = "You opened the second tab!";
    tab->addChild(lbl);

    return tab;
}

Shared<BaseWidget> createTab3() {
    auto tab = MakeRef<SplitterPanel>();
    tab->backgroundColor = Color(100, 160, 80);
    tab->sectionWeights = "1 3 1";

    for (int i = 0; i < 3; i++) {
        auto lbl = MakeRef<Label>();
        lbl->marginLeft = 80;
        lbl->marginRight = 10;
        lbl->marginTop = 10;
        lbl->marginBottom = 10;
        lbl->text = "You opened the third tab :D";
        tab->addChild(lbl);
    }

    return tab;
}

Shared<BaseWidget> createTab4() {
    auto tab = MakeRef<StackPanel>();
    tab->backgroundColor = Color(40, 16, 80);

    auto cbx = MakeRef<Checkbox>();
    cbx->label->text = "4th tab checkbox!";
    cbx->marginLeft = 50;
    cbx->marginTop = 50;
    tab->addChild(cbx);

    return tab;
}

int main() {
    AppManager::registerApplication("appId-tabview-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "TabView Demo");

    auto tab1 = createTab1();
    auto tab2 = createTab2();
    auto tab3 = createTab3();
    auto tab4 = createTab4();

    auto root = MakeRef<TabView>();
    root->addTab("Settings", tab1);
    root->addTab("Project", tab2);
    root->addTab("somefile.h", tab3);
    root->addTab("another_file.cpp", tab4);
    window->setRootWidget(root);

    AppManager::startApplicationLoop();
    return 0;
}
