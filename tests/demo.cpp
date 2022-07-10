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

    auto overlay = MakeRef<Overlay>();

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

    auto panel = MakeRef<Panel>();
    panel->position = { 400, 300 };
    panel->size = { 300, 300 };
    panel->backgroundColor = Color(10, 20, 10);
    window->addWidget(panel);

    auto btn2 = MakeRef<Button>();
    btn2->position = { 30, 80 };
    btn2->size = { 200, 24 };
    btn2->text = "See selected text";
    btn2->on("clicked", [&](Shared<Event> e) {
        if (!entry->hasSelectedText()) {
            return;
        }

        auto text = entry->getSelectedText();

        auto b = MakeRef<Button>();
        b->position = { 40, 40 };
        b->text = text;
        b->on("clicked", [overlay](auto e) {
            overlay->hide();
        });
        overlay->setContent(b);

        auto btnPosInWindow = btn2->getPositionInWindow();

        auto clickPos = std::static_pointer_cast<MouseButtonEvent>(e)->getLocation();
        auto screenClickPos = std::static_pointer_cast<MouseButtonEvent>(e)->getScreenLocation();

        Position anchorDiff = { clickPos.x - btnPosInWindow.x, clickPos.y - btnPosInWindow.y };

        Point anchorPoint = {
            screenClickPos.x - anchorDiff.x,
            screenClickPos.y - anchorDiff.y + (int32_t)btn2->size->height
        };

        overlay->setSize(btn2->size->width, 120);
        overlay->setAnchor(anchorPoint);
        overlay->show();
    });
    panel->addChild(btn2);

    AppManager::startApplicationLoop();
    return 0;
}
