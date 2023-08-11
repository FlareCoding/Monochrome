#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <window/ModernWindow.h>
#include <window/Overlay.h>
#include <widgets/Color.h>
#include <widgets/AllWidgets.h>
using namespace mc;

#include <rendering/Renderer.h>
#include <events/KeyboardEvents.h>
#include <utils/FileDialog.h>

int main() {
    AppManager::registerApplication("appId-041587");
    //Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1180, 860, "Monochrome Demo");
    BaseWidget::setUniversalBackgroundColor(window->getBackgroundColor());

    auto root = MakeRef<StackPanel>();
    window->setRootWidget(root);

    auto label = MakeRef<Label>("Monochrome Widgets Demo");
    label->marginTop = 6;
    label->marginBottom = 12;
    root->addChild(label);

    auto demoGroupsPanel = MakeRef<StackPanel>();
    demoGroupsPanel->marginLeft = 40;
    demoGroupsPanel->marginRight = 40;
    root->addChild(demoGroupsPanel);

    auto buttonsGroupExpander = MakeRef<Expander>("Buttons");
    buttonsGroupExpander->header->label->fontSize = 12;
    buttonsGroupExpander->expanded = true;
    demoGroupsPanel->addChild(buttonsGroupExpander);

    auto defaultButton = MakeRef<Button>("Click Me");
    defaultButton->marginRight = 6;

    auto disabledButton = MakeRef<Button>("Disabled");
    disabledButton->marginRight = 10;
    disabledButton->fixedWidth = 150;
    disabledButton->enabled = false;

    auto checkbox = MakeRef<Checkbox>("Enabled State");
    checkbox->marginRight = 10;
    checkbox->on("valueChanged", [disabledButton](Shared<Event> e) {
        auto checked = e->get<bool>("checked");
        disabledButton->enabled = checked;
        disabledButton->label->text = checked ? "Enabled" : "Disabled";
    });

    auto iconButton = MakeRef<Button>("Open Folder");
    iconButton->marginRight = 6;
    iconButton->setImage(Image::loadFromWebUrl("https://img.icons8.com/?size=512&id=c2AXPLZ3iVEU&format=png"));
    iconButton->on("clicked", [](Shared<Event> e) {
        auto dialog = utils::FileDialog::create();
        dialog->chooseFolderDialog();
    });

    auto buttonsGroup = MakeRef<StackPanel>();
    buttonsGroup->marginTop = 6;
    buttonsGroup->marginBottom = 6;
    buttonsGroup->orientation = Horizontal;
    buttonsGroup->addChild(defaultButton);
    buttonsGroup->addChild(disabledButton);
    buttonsGroup->addChild(checkbox);
    buttonsGroup->addChild(iconButton);

    auto buttonsGroupContainer = MakeRef<StackPanel>();
    buttonsGroupContainer->backgroundColor = Color(30, 30, 31);
    buttonsGroupContainer->addChild(buttonsGroup);
    buttonsGroupExpander->setContent(buttonsGroupContainer);

    AppManager::startApplicationLoop();
    return 0;
}
