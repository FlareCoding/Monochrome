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

    auto treeView = MakeRef<TreeView>();
    treeView->backgroundColor = Color(31, 39, 46);
    treeView->itemTextColor = Color(0, 200, 200);
    treeView->itemHighlightedBorderColor = Color::green;
    treeView->minWidth = 280;
    treeView->on("itemSelected", [](Shared<Event> e) {
        auto groupAction = e->get<std::string>("groupAction");
        if (groupAction == "none") {
            printf("TreeView item selected: %s\n", e->get<std::string>("item").c_str());
        } else {
            printf("TreeView group %s: %s\n", groupAction.c_str(), e->get<std::string>("item").c_str());
        }
    });
    scrollView->addChild(treeView);

    auto miscGroup = MakeRef<TreeViewNode>("Misc.", "misc");
    miscGroup->addChild("New Gen AirPods Max + iPad Pro (Dual Camera) + Apple Pencil");
    miscGroup->addChild("iPad Mini (Vintage)", "ipmv");
    miscGroup->addChild("iPhone 5SE (Discountinued)", "ip5se");

    auto appleLaptopsGroup = MakeRef<TreeViewNode>("Apple");
    appleLaptopsGroup->addChild("Macbook Pro 13-inch");
    appleLaptopsGroup->addChild("Macbook Pro 15-inch");
    appleLaptopsGroup->addChild("Macbook Air", "mcbpair");
    appleLaptopsGroup->addChild(miscGroup);

    auto laptopsGroup = MakeRef<TreeViewNode>("Laptops", "laptops");
    laptopsGroup->addChild("Dell");
    laptopsGroup->addChild(appleLaptopsGroup);
    laptopsGroup->addChild("HP");
    laptopsGroup->addChild("Acer");

    auto phonesGroup = MakeRef<TreeViewNode>("Phones");
    phonesGroup->addChild("iPhone 12");
    phonesGroup->addChild("iPhone 13");
    phonesGroup->addChild("iPhone 13 Pro");
    phonesGroup->addChild("Samsung Galaxy s22");
    phonesGroup->addChild("Nokia flip-phone");

    auto rootNode = MakeRef<TreeViewNode>("", "root");
    rootNode->addChild(laptopsGroup);
    rootNode->addChild(phonesGroup);

    treeView->setRootNode(rootNode);
    treeView->expandNodeByKey("laptops");

    AppManager::startApplicationLoop();
    return 0;
}
