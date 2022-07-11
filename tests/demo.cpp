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
    overlay->spawnDirection = Right;
    overlay->setActivatorWidget(dropdownButton);

    auto dropdownButton2 = MakeRef<Button>();
    dropdownButton2->position = { 30, 30 };
    dropdownButton2->text = "Open 2";
    overlay->setContent(dropdownButton2);

    auto overlay2 = MakeRef<Overlay>();
    overlay2->setSize(200, 140);
    //overlay->spawnDirection = Left;
    overlay2->setActivatorWidget(dropdownButton2);

    AppManager::startApplicationLoop();
    return 0;
}
