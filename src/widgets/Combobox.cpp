#include "Combobox.h"

namespace mc {
    Combobox::Combobox() : Button() {
        _setupProperties();
    }

    void Combobox::_onSetComputedSize(const Size& size) {
        Button::_onSetComputedSize(size);

        d_menuList->measure();
        auto overlaySize = d_menuList->getDesiredSize();

        if (size.width > overlaySize.width) {
            overlaySize.width = size.width;
        }

        d_menuList->setComputedSize(overlaySize);
    }

    void Combobox::addItem(const std::string& item) {
        // If the first item was added, set the
        // MenuList's activator to this combobox.
        if (getItemCount() == 0) {
            d_menuList->setActivatorWidget(this, OverlayActivationType::OnClick);
            label->text = item;
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
            label->text = d_selectedItem;
        }

        return successfulRemoval;
    }

    void Combobox::removeAllItems() {
        d_menuList->removeAllItems();
        d_selectedItem = "";
        label->text = d_selectedItem;
    }

    std::string Combobox::getSelectedItem() {
        return d_selectedItem;
    }

    void Combobox::_setupProperties() {
        d_menuList = MakeRef<MenuList>();
        d_menuList->forwardEmittedEvent(this, "itemSelected");
        d_menuList->on("itemSelected", [this](Shared<Event> e) {
            // Update the record of the selected item
            d_selectedItem = e->get<std::string>("item");

            // Update the displayed text
            label->text = d_selectedItem;
        });

        overlayBorderColor.on("propertyChanged", [this](auto e) {
            d_menuList->borderColor = overlayBorderColor;
        });

        overlayBorderThickness.on("propertyChanged", [this](auto e) {
            d_menuList->borderThickness = overlayBorderThickness;
        });

        itemSize.on("propertyChanged", [this](auto e) {
            d_menuList->itemSize = itemSize;
        });

        overlaySpawnDirection.on("propertyChanged", [this](auto e) {
            d_menuList->spawnDirection = overlaySpawnDirection;
        });

        itemColor.on("propertyChanged", [this](auto e) {
            d_menuList->itemTextColor = itemColor;
        });

        itemBackgroundColor.on("propertyChanged", [this](auto e) {
            d_menuList->itemBackgroundColor = itemBackgroundColor;
        });

        overlayBorderColor = Color::black;
        handleWidgetVisiblePropertyChange(overlayBorderColor);

        overlayBorderThickness = 1;
        handleWidgetVisiblePropertyChange(overlayBorderThickness);

        itemSize = 12;
        handleWidgetVisiblePropertyChange(itemSize);

        overlaySpawnDirection = OverflowDirection::Down;
        handleWidgetVisiblePropertyChange(overlaySpawnDirection);

        itemColor = Color::white;
        handleWidgetVisiblePropertyChange(itemColor);

        itemBackgroundColor = Color::black;
        handleWidgetVisiblePropertyChange(itemBackgroundColor);

        label->color = Color::black;
        label->text = "";
        backgroundColor = Color::lightGray;
        itemBackgroundColor = Color::darkGray;

        // A manual utf8 buffer has to be used because of
        // a compiler bug that misinterprets a string literal.
        // The secondary text represents a dropdown arrow: '▼'.
        const char secondaryTextBuffer[4] = { -30, -106, -68, 0 };
        secondaryRightText = secondaryTextBuffer;
    }
}  // namespace mc
