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
    treeView->minWidth = 280;
    treeView->on("itemSelected", [](Shared<Event> e) {
        printf("TreeView item selected: %s\n", e->get<std::string>("item").c_str());
    });
    scrollView->addChild(treeView);

    auto miscGroup = MakeRef<TreeViewGroup>("Misc.");
    miscGroup->addItem("New Gen AirPods Max + iPad Pro (Dual Camera) + Apple Pencil");
    miscGroup->addItem("iPad Mini (Vintage)");
    miscGroup->addItem("iPhone 5SE (Discountinued)");

    auto appleLaptopsGroup = MakeRef<TreeViewGroup>("Apple");
    appleLaptopsGroup->addItem("Macbook Pro 13-inch");
    appleLaptopsGroup->addItem("Macbook Pro 15-inch");
    appleLaptopsGroup->addItem("Macbook Air");
    appleLaptopsGroup->addSubGroup(miscGroup);

    auto laptopsGroup = MakeRef<TreeViewGroup>("Laptops");
    laptopsGroup->addItem("Dell");
    laptopsGroup->addSubGroup(appleLaptopsGroup);
    laptopsGroup->addItem("HP");
    laptopsGroup->addItem("Acer");
    treeView->addGroup(laptopsGroup);

    auto phonesGroup = MakeRef<TreeViewGroup>("Phones");
    phonesGroup->addItem("iPhone 12");
    phonesGroup->addItem("iPhone 13");
    phonesGroup->addItem("iPhone 13 Pro");
    phonesGroup->addItem("Samsung Galaxy s22");
    phonesGroup->addItem("Nokia flip-phone");
    treeView->addGroup(phonesGroup);

    AppManager::startApplicationLoop();
    return 0;
}
