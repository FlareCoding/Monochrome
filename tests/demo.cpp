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

    auto dropdownButton = MakeRef<Button>();
    dropdownButton->position = { 340, 130 };
    dropdownButton->text = "Open";
    dropdownButton->cornerRadius = 0;
    window->addWidget(dropdownButton);

    auto moreMenu = MakeRef<MenuList>("More...");
    moreMenu->addMenuItem("Save");
    moreMenu->addMenuItem("Save As");
    moreMenu->addMenuItem("Open");
    moreMenu->on("itemSelected", itemSelected);
    moreMenu->backgroundColor = Color(60, 64, 62);
    moreMenu->color = Color::yellow;
    moreMenu->borderColor = Color::yellow;

    auto ddMenuList = MakeRef<MenuList>();
    ddMenuList->on("itemSelected", itemSelected);

    ddMenuList->backgroundColor = Color(60, 64, 62);
    ddMenuList->setActivatorWidget(dropdownButton);
    ddMenuList->addMenuItem("New Project");
    ddMenuList->addMenu(moreMenu);
    ddMenuList->addMenuItem("Exit");
    ddMenuList->color = Color::green;
    ddMenuList->borderColor = Color::blue;
    ddMenuList->borderThickness = 2;
    ddMenuList->size = { 200, 0 };

    AppManager::startApplicationLoop();
    return 0;
}
