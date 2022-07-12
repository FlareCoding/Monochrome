#include "MenuList.h"
#include "Button.h"
#include <window/Overlay.h>

namespace mc
{
    MenuList::MenuList() {
        _setupProperties();
    }

    MenuList::MenuList(const std::string& name) : name(name) {
        _setupProperties();
    }

    void MenuList::_setupProperties() {
        backgroundColor = Color(20, 20, 20);
        color = Color::white;

        name = "Item 1";
        name.forwardEmittedEvents(this);

        size.on("propertyChanged", [this](auto e) {
            d_overlay->setSize(size->width, size->height);
            d_contentPanel->size = size;
        });

        backgroundColor.on("propertyChanged", [this](auto e) {
            d_contentPanel->backgroundColor = backgroundColor;
        });

        // Create the content panel
        d_contentPanel = MakeRef<Panel>();
        d_contentPanel->cornerRadius = 0;
        d_contentPanel->backgroundColor = Color::black;

        // Create the overlay
        d_overlay = MakeRef<Overlay>();
        d_overlay->spawnDirection = OverflowDirection::Right;
        d_overlay->setContent(d_contentPanel);

        // Setup a default size
        size = { 140, 0 };
    }

    void MenuList::addMenu(Shared<MenuList> menu) {
        auto menuButton = MakeRef<Button>();
        menuButton->position = { 0, (int32_t)d_menuItems.size() };
        menuButton->size = { size->width, d_itemSize };
        menuButton->cornerRadius = 0;
        menuButton->text = menu->name;
        menuButton->color = color;
        menuButton->backgroundColor = backgroundColor;
        d_contentPanel->addChild(menuButton);

        // Set the button as menu list activator
        menu->setActivatorWidget(menuButton);

        // Set the menu's overlay as the child overlay
        d_overlay->addChildOverlay(menu->d_overlay);

        // Add the menu to the list
        d_menuItems.insert({ menu->name, menu });

        // Adjust the overall size of the menulist display
        this->size = { size->width, size->height + d_itemSize };
    }

    void MenuList::addMenuItem(const MenuItem& item) {
        auto menuItemButton = MakeRef<Button>();
        menuItemButton->position = { 0, (int32_t)(d_menuItems.size() * d_itemSize) };
        menuItemButton->size = { size->width, d_itemSize };
        menuItemButton->cornerRadius = 0;
        menuItemButton->text = item.name;
        menuItemButton->color = color;
        menuItemButton->backgroundColor = backgroundColor;
        d_contentPanel->addChild(menuItemButton);

        // Add the menu item to the list
        d_menuItems.insert({ item.name, item });

        // Adjust the overall size of the menulist display
        this->size = { size->width, size->height + d_itemSize };
    }
    
    void MenuList::setActivatorWidget(Shared<BaseWidget> widget) {
        d_overlay->setActivatorWidget(widget);
    }
}
