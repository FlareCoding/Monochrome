#include "Combobox.h"

namespace mc {
    Combobox::Combobox() : Button() {
        _setupComboboxProperties();
        _setupEventHandlers();
    }

    void Combobox::addItem(const std::string& item) {
        // If the first item was added, set the
        // MenuList's activator to this combobox.
        if (getItemCount() == 0) {
            d_menuList->setActivatorWidget(this);
            text = item;
            d_selectedItem = item;
        }

        d_menuList->addMenuItem(item);
    }

    void Combobox::addItems(const std::initializer_list<std::string>& items) {
        for (auto& item : items) {
            addItem(item);
        }
    }

    void Combobox::addItems(const std::vector<std::string>& items) {
        for (auto& item : items) {
            addItem(item);
        }
    }

    bool Combobox::removeItem(const std::string& item) {
        bool successfulRemoval = d_menuList->removeItem(item);

        // If the element that was displayed got removed, replace the displayed
        // text to either blank or the first element depending on the item count.
        if (successfulRemoval && item == d_selectedItem) {
            d_selectedItem = d_menuList->getItemCount() ? d_menuList->getItemName(0) : "";
            text = d_selectedItem;
        }

        return successfulRemoval;
    }

    void Combobox::removeAllItems() {
        d_menuList->removeAllItems();
        d_selectedItem = "";
        text = d_selectedItem;
    }

    void Combobox::_setupComboboxProperties() {
        overlayBorderColor = Color::black;
        overlayBorderColor.forwardEmittedEvents(this);

        borderThickness = 1;
        borderThickness.forwardEmittedEvents(this);

        itemSize = 24;
        itemSize.forwardEmittedEvents(this);

        overlaySpawnDirection = OverflowDirection::Down;
        overlaySpawnDirection.forwardEmittedEvents(this);

        itemColor = Color::black;
        itemColor.forwardEmittedEvents(this);

        color = Color::black;
        backgroundColor = Color::lightGray;
        text = "";
        secondaryRightText = u8"\u25BC ";  // Down arrow

        d_menuList = MakeRef<MenuList>();
        d_menuList->forwardEmittedEvent(this, "itemSelected");
        d_menuList->spawnDirection = overlaySpawnDirection;
        d_menuList->size = { 0, 0 };
        d_menuList->color = itemColor;
        d_menuList->backgroundColor = backgroundColor;
        d_menuList->borderColor = overlayBorderColor;
    }

    void Combobox::_setupEventHandlers() {
        d_menuList->on("itemSelected", [this](Shared<Event> e) {
            // Update the record of the selected item
            d_selectedItem = e->get<std::string>("item");

            // Update the displayed text
            text = d_selectedItem;
        });

        size.on("propertyChanged", [this](auto e) {
            d_menuList->size = size;
        });

        overlayBorderColor.on("propertyChanged", [this](auto e) {
            d_menuList->borderColor = overlayBorderColor;
        });

        borderThickness.on("propertyChanged", [this](auto e) {
            d_menuList->borderThickness = borderThickness;
        });

        itemSize.on("propertyChanged", [this](auto e) {
            d_menuList->menuItemSize = itemSize;
        });

        overlaySpawnDirection.on("propertyChanged", [this](auto e) {
            d_menuList->spawnDirection = overlaySpawnDirection;
        });

        itemColor.on("propertyChanged", [this](auto e) {
            d_menuList->color = itemColor;
        });

        backgroundColor.on("propertyChanged", [this](auto e) {
            d_menuList->backgroundColor = backgroundColor;
        });
    }
}  // namespace mc
