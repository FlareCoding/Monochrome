#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/AllWidgets.h>
using namespace mc;

void itemSelected(Shared<Event> e) {
    auto item = e->get<MenuItem>("item");
    printf("Item Selected: %s\n", item.c_str());
}

uint32_t random(int min, int max) {
    static bool first = true;
    if (first) {
        srand((uint32_t)time(NULL)); //seeding for the first time only!
        first = false;
    }
    return (uint32_t)(min + rand() % ((max + 1) - min));
}

int main()
{
    AppManager::registerApplication("appId-032487");

    auto window = MakeRef<ModernWindow>(1060, 660, "Monochrome Demo");
    window->setBackgroundColor(Color(18, 22, 28));

    auto entry = MakeRef<Entry>();
    entry->position = { 140, 80 };
    window->addWidget(entry);

    auto btn = MakeRef<Button>();
    btn->position = { 140, 130 };
    btn->text = "Generate text";
    btn->on("clicked", [entry](auto e) {
        entry->text = "this is a test long text";
    });
    window->addWidget(btn);

    auto extraMenu = MakeRef<MenuList>("Actions...");
    extraMenu->addMenuItem("Like");
    extraMenu->addMenuItem("Notify");
    extraMenu->addMenuItem("Subscribe");
    extraMenu->on("itemSelected", itemSelected);
    extraMenu->backgroundColor = Color(60, 64, 62);
    extraMenu->color = Color(195, 255, 100);
    extraMenu->borderColor = Color::red;

    auto moreMenu = MakeRef<MenuList>("More...");
    moreMenu->addMenuItem("Save");
    moreMenu->addMenuItem("Save As");
    moreMenu->addMenuItem("Open");
    moreMenu->addSubMenu(extraMenu);
    moreMenu->on("itemSelected", itemSelected);
    moreMenu->backgroundColor = Color(60, 64, 62);
    moreMenu->color = Color::yellow;
    moreMenu->borderColor = Color::yellow;

    auto ddMenuList = MakeRef<MenuList>();
    ddMenuList->on("itemSelected", itemSelected);
    ddMenuList->backgroundColor = Color(60, 64, 62);
    ddMenuList->addMenuItem("New Project");
    ddMenuList->addSubMenu(moreMenu);
    ddMenuList->addMenuItem("Exit");
    ddMenuList->color = Color::green;
    ddMenuList->borderColor = Color::blue;
    ddMenuList->borderThickness = 2;
    ddMenuList->size = { 200, 0 };

    auto dropdown = MakeRef<DropdownButton>();
    dropdown->position = { 340, 130 };
    dropdown->size = { 160, 20 };
    dropdown->text = "Open Menu";
    dropdown->cornerRadius = 0;
    dropdown->setMenuList(ddMenuList);
    window->addWidget(dropdown);

    auto flowContainer = MakeRef<FlowPanel>();
    flowContainer->position = { 140, 240 };
    flowContainer->size = { 540, 300 };
    flowContainer->cornerRadius = 2;
    flowContainer->backgroundColor = Color(20, 50, 20);
    flowContainer->layout = Horizontal;
    flowContainer->justifyContent = None;
    flowContainer->stretchContents = false;
    flowContainer->autoSize = false;
    window->addWidget(flowContainer);

    auto addBtn = MakeRef<Button>();
    addBtn->cornerRadius = 2;
    addBtn->text = "Add";
    addBtn->on("clicked", [flowContainer](auto e) {
        static int counter = 2;
        ++counter;

        auto b = MakeRef<Button>();
        b->size = { random(100, 170), random(30, 60) };
        b->cornerRadius = 2;
        b->text = "Button " + std::to_string(counter);
        flowContainer->addChild(b);
    });
    flowContainer->addChild(addBtn);

    auto clearBtn = MakeRef<Button>();
    clearBtn->cornerRadius = 2;
    clearBtn->text = "Clear";
    clearBtn->on("clicked", [flowContainer](auto e) {
        if (flowContainer->layout == Horizontal ||
            flowContainer->layout == HorizontalReversed
        ) {
            while (flowContainer->getChildren().size() > 2) {
                flowContainer->removeChild(flowContainer->getChild(2));
            }
        }
    });
    flowContainer->addChild(clearBtn);

    auto layoutCombobox = MakeRef<Combobox>();
    layoutCombobox->position = { 700, 240 };
    layoutCombobox->size = { 160, 20 };
    layoutCombobox->cornerRadius = 0;
    layoutCombobox->addItems({ "Horizontal", "HorizontalReversed", "Vertical", "VerticalReversed" });
    layoutCombobox->overlayBorderColor = Color::white;
    layoutCombobox->itemColor = Color::white;
    layoutCombobox->color = Color::white;
    layoutCombobox->backgroundColor = Color::darkGray;
    layoutCombobox->overlaySpawnDirection = Right;
    layoutCombobox->on("itemSelected", [flowContainer](Shared<Event> e) {
        auto item = e->get<std::string>("item");
        if (item == "Horizontal") {
            flowContainer->layout = Horizontal;
        } else if (item == "HorizontalReversed") {
            flowContainer->layout = HorizontalReversed;
        } else if (item == "Vertical") {
            flowContainer->layout = Vertical;
        } else if (item == "VerticalReversed") {
            flowContainer->layout = VerticalReversed;
        }
    });
    window->addWidget(layoutCombobox);

    auto justifyContentCombobox = MakeRef<Combobox>();
    justifyContentCombobox->position = { 700, 270 };
    justifyContentCombobox->size = { 160, 20 };
    justifyContentCombobox->cornerRadius = 0;
    justifyContentCombobox->addItems({ "None", "Fill", "Center", "SpaceBetween", "SpaceAround" });
    justifyContentCombobox->overlayBorderColor = Color::white;
    justifyContentCombobox->itemColor = Color::white;
    justifyContentCombobox->color = Color::white;
    justifyContentCombobox->backgroundColor = Color::darkGray;
    justifyContentCombobox->overlaySpawnDirection = Right;
    justifyContentCombobox->on("itemSelected", [flowContainer](Shared<Event> e) {
        auto item = e->get<std::string>("item");
        if (item == "None") {
            flowContainer->justifyContent = None;
        }
        else if (item == "Fill") {
            flowContainer->justifyContent = Fill;
        }
        else if (item == "Center") {
            flowContainer->justifyContent = Center;
        }
        else if (item == "SpaceBetween") {
            flowContainer->justifyContent = SpaceBetween;
        }
        else if (item == "SpaceAround") {
            flowContainer->justifyContent = SpaceAround;
        }
    });
    window->addWidget(justifyContentCombobox);

    auto resizeFlowContainerBtn = MakeRef<Button>();
    resizeFlowContainerBtn->position = { 700, 310 };
    resizeFlowContainerBtn->size = { 200, 26 };
    resizeFlowContainerBtn->text = "Resize FlowPanel";
    resizeFlowContainerBtn->on("clicked", [flowContainer](auto e) {
        flowContainer->size = { flowContainer->size->width, random(100, 200) };
    });
    window->addWidget(resizeFlowContainerBtn);

    AppManager::startApplicationLoop();
    return 0;
}
