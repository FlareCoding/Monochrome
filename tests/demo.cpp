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

Shared<MenuList> createMenuList() {
    auto extraMenu = MakeRef<MenuList>("Actions...");
    extraMenu->addMenuItem("Like");
    extraMenu->addMenuItem("Notify");
    extraMenu->addMenuItem("Subscribe");
    extraMenu->on("itemSelected", [](Shared<Event> e) {
        auto item = e->get<MenuItem>("item");
        printf("Item Selected: %s\n", item.c_str());
    });
    extraMenu->borderColor = Color::red;
    extraMenu->itemTextColor = Color::yellow;

    auto moreMenu = MakeRef<MenuList>("More...");
    moreMenu->addMenuItem("Save");
    moreMenu->addMenuItem("Save As");
    moreMenu->addMenuItem("Open");
    moreMenu->addSubMenu(extraMenu);
    moreMenu->on("itemSelected", [](Shared<Event> e) {
        auto item = e->get<MenuItem>("item");
        printf("Item Selected: %s\n", item.c_str());
    });
    moreMenu->borderColor = Color::yellow;
    moreMenu->itemTextColor = Color::white;

    auto zoomMenu = MakeRef<MenuList>("Zoom...");
    zoomMenu->addMenuItem("200%");
    zoomMenu->addMenuItem("150%");
    zoomMenu->addMenuItem("100%");
    zoomMenu->addMenuItem("75%");
    zoomMenu->on("itemSelected", [](Shared<Event> e) {
        auto item = e->get<MenuItem>("item");
        printf("Item Selected: %s\n", item.c_str());
    });
    zoomMenu->borderColor = Color::white;

    auto mainMenuList = MakeRef<MenuList>();
    mainMenuList->on("itemSelected", [](Shared<Event> e) {
        auto item = e->get<MenuItem>("item");
        printf("Item Selected: %s\n", item.c_str());
    });
    mainMenuList->addMenuItem("New Project");
    mainMenuList->addSubMenu(moreMenu);
    mainMenuList->addSubMenu(zoomMenu);
    mainMenuList->addMenuItem("Exit");
    mainMenuList->borderColor = Color::blue;
    mainMenuList->itemTextColor = Color::green;
    mainMenuList->borderThickness = 2;

    return mainMenuList;
}

int main() {
    AppManager::registerApplication("appId-041587");
    //Renderer::enableDebugBoundingBoxes = true;

    auto window = MakeRef<ClassicWindow>(1180, 860, "Monochrome Demo");

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

    auto buttonsGroupExpander = MakeRef<Expander>("Buttons Group");
    buttonsGroupExpander->header->label->fontSize = 12;
    buttonsGroupExpander->expanded = true;
    demoGroupsPanel->addChild(buttonsGroupExpander);

    auto defaultButton = MakeRef<Button>("Click Me");
    defaultButton->marginLeft = 40;
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

    auto dropdownButton = MakeRef<DropdownButton>("Dropdown");
    dropdownButton->marginRight = 6;
    dropdownButton->secondaryRightText = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xbc),
        static_cast<char>(0x20),
        0
    };
    dropdownButton->setMenuList(createMenuList());

    auto buttonsGroup = MakeRef<StackPanel>();
    buttonsGroup->marginTop = 6;
    buttonsGroup->marginBottom = 6;
    buttonsGroup->orientation = Horizontal;
    buttonsGroup->addChild(defaultButton);
    buttonsGroup->addChild(disabledButton);
    buttonsGroup->addChild(checkbox);
    buttonsGroup->addChild(iconButton);
    buttonsGroup->addChild(dropdownButton);

    auto buttonsGroupContainer = MakeRef<StackPanel>();
    buttonsGroupContainer->backgroundColor = Color(30, 30, 31);
    buttonsGroupContainer->marginBottom = 20;
    buttonsGroupContainer->addChild(buttonsGroup);
    buttonsGroupExpander->setContent(buttonsGroupContainer);

    auto entriesGroupExpander = MakeRef<Expander>("Entries Group");
    entriesGroupExpander->header->label->fontSize = 12;
    entriesGroupExpander->expanded = true;
    demoGroupsPanel->addChild(entriesGroupExpander);

    auto defaultEntry = MakeRef<Entry>();
    defaultEntry->marginLeft = 40;
    defaultEntry->marginRight = 12;

    auto readOnlyEntry = MakeRef<Entry>("Sample text is here");
    readOnlyEntry->marginRight = 12;
    readOnlyEntry->readOnly = true;
    readOnlyEntry->borderThickness = 1;
    readOnlyEntry->borderColor = Color::white;

    auto entriesGroup = MakeRef<StackPanel>();
    entriesGroup->marginTop = 6;
    entriesGroup->marginBottom = 6;
    entriesGroup->orientation = Horizontal;
    entriesGroup->addChild(defaultEntry);
    entriesGroup->addChild(readOnlyEntry);

    auto entriesGroupContainer = MakeRef<StackPanel>();
    entriesGroupContainer->backgroundColor = Color(30, 30, 31);
    entriesGroupContainer->addChild(entriesGroup);
    entriesGroupExpander->setContent(entriesGroupContainer);

    AppManager::startApplicationLoop();
    return 0;
}
