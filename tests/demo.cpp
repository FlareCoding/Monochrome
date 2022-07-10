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

    auto dropdownButton = MakeRef<Button>();
    dropdownButton->position = { 340, 130 };
    dropdownButton->text = "Open";
    window->addWidget(dropdownButton);

    auto overlay = MakeRef<Overlay>();
    overlay->setSize(200, 140);
    overlay->setActivatorWidget(dropdownButton);

    AppManager::startApplicationLoop();
    return 0;
}
