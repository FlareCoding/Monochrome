#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/AllWidgets.h>
using namespace mc;

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

    auto fileMenu = MakeRef<MenuList>("File");
    fileMenu->backgroundColor = Color(60, 64, 62);
    fileMenu->color = Color::yellow;
    fileMenu->addMenuItem(MenuItem("Save"));
    fileMenu->addMenuItem(MenuItem("Save As"));
    fileMenu->addMenuItem(MenuItem("Open"));

    MenuItem newItem("New");

    auto ddMenuList = MakeRef<MenuList>();

    auto dropdownButton = MakeRef<Button>();
    dropdownButton->position = { 340, 130 };
    dropdownButton->text = "Open";
    dropdownButton->cornerRadius = 0;
    window->addWidget(dropdownButton);

    ddMenuList->backgroundColor = Color(60, 64, 62);
    ddMenuList->color = Color::yellow;
    ddMenuList->setActivatorWidget(dropdownButton);
    ddMenuList->addMenu(fileMenu);
    ddMenuList->addMenuItem(newItem);

    AppManager::startApplicationLoop();
    return 0;
}
