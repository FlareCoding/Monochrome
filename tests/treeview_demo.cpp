#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
using namespace mc;

auto makeButton() {
    auto button = MakeRef<Button>();
    button->cornerRadius = 0;
    button->borderThickness = 3;
    button->borderColor = Color::randomColor();

    return button;
}

int main() {
    AppManager::registerApplication("appId-treeview-demo");

    auto window = MakeRef<ClassicWindow>(860, 640, "TreeView Demo");

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto scrollView = MakeRef<ScrollPanel>();
    scrollView->marginTop = 20;
    scrollView->horizontalAlignment = HACenter;
    scrollView->fixedWidth = 280;
    rootPanel->addChild(scrollView);

    auto treeView = MakeRef<TreeView2>();
    treeView->backgroundColor = Color(31, 39, 46);
    treeView->minWidth = 280;
    // treeView->on("itemSelected", [](Shared<Event> e) {
    //     printf("TreeView item selected: %s\n", e->get<std::string>("item").c_str());
    // });
    scrollView->addChild(treeView);

    auto miscGroup = MakeRef<TreeViewNode>("Misc.", "misc");
    miscGroup->addChild("New Gen AirPods Max + iPad Pro (Dual Camera) + Apple Pencil", "ngamip");
    miscGroup->addChild("iPad Mini (Vintage)", "ipmv");
    miscGroup->addChild("iPhone 5SE (Discountinued)", "ip5se");

    auto appleLaptopsGroup = MakeRef<TreeViewNode>("Apple", "apple");
    appleLaptopsGroup->addChild("Macbook Pro 13-inch", "mcbp13");
    appleLaptopsGroup->addChild("Macbook Pro 15-inch", "mcbp15");
    appleLaptopsGroup->addChild("Macbook Air", "mcbpair");
    appleLaptopsGroup->addChild(miscGroup);

    auto laptopsGroup = MakeRef<TreeViewNode>("Laptops", "laptops");
    laptopsGroup->addChild("Dell", "dell");
    laptopsGroup->addChild(appleLaptopsGroup);
    laptopsGroup->addChild("HP", "hp");
    laptopsGroup->addChild("Acer", "acer");

    auto phonesGroup = MakeRef<TreeViewNode>("Phones", "phones");
    phonesGroup->addChild("iPhone 12", "ip12");
    phonesGroup->addChild("iPhone 13", "ip13");
    phonesGroup->addChild("iPhone 13 Pro", "ip13p");
    phonesGroup->addChild("Samsung Galaxy s22", "sgs22");
    phonesGroup->addChild("Nokia flip-phone", "nfp");

    auto rootNode = MakeRef<TreeViewNode>("", "root");
    rootNode->addChild(laptopsGroup);
    rootNode->addChild(phonesGroup);

    treeView->setRootNode(rootNode);

    AppManager::startApplicationLoop();
    return 0;
}
