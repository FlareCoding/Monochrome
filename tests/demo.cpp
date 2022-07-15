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

int main()
{
    AppManager::registerApplication("appId-032487");

    auto window = MakeRef<ModernWindow>(860, 660, "Monochrome Demo");
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

    auto combobox = MakeRef<Combobox>();
    combobox->position = { 540, 100 };
    combobox->size = { 160, 20 };
    combobox->cornerRadius = 0;
    combobox->addItems({ "Eggs", "Milk", "Beef", "Lettuce", "Tomatos" });
    combobox->overlayBorderColor = Color::white;
    combobox->itemColor = Color::white;
    combobox->color = Color::white;
    combobox->backgroundColor = Color::darkGray;
    window->addWidget(combobox);

    AppManager::startApplicationLoop();
    return 0;
}
