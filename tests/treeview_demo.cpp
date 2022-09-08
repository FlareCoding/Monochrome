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
    AppManager::registerApplication("appId-monodev-devtools");

    auto window = MakeRef<ClassicWindow>(860, 640, "DockPanel Demo");

    auto rootPanel = MakeRef<StackPanel>();
    rootPanel->backgroundColor = Color(18, 22, 28);
    window->setRootWidget(rootPanel);

    auto treeView = MakeRef<TreeView>();
    treeView->marginTop = 20;
    treeView->horizontalAlignment = HACenter;
    treeView->backgroundColor = Color(31, 39, 46);
    treeView->fixedWidth = 280;
    treeView->on("itemSelected", [](Shared<Event> e) {
        printf("TreeView item selected: %s\n", e->get<std::string>("item").c_str());
    });
    rootPanel->addChild(treeView);

    auto macLaptopsGroup = MakeRef<TreeViewGroup>("Apple");
    macLaptopsGroup->addItem("Macbook Pro 13-inch");
    macLaptopsGroup->addItem("Macbook Pro 15-inch");
    macLaptopsGroup->addItem("Macbook Air");

    auto laptopsGroup = MakeRef<TreeViewGroup>("Laptops");
    laptopsGroup->addItem("Dell");
    laptopsGroup->addSubGroup(macLaptopsGroup);
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
