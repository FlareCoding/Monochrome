#pragma once
#include "Button.h"
#include "MenuList.h"

namespace mc
{
    class Combobox : public Button
    {
    public:
        Combobox();

        // Specifies the preferred orientation for spawning
        // the overlay window around the combobox widget.
        PropertyObserver<OverflowDirection> overlaySpawnDirection;

        // Color of the border around the MenuList overlay
        PropertyObserver<Color>             overlayBorderColor;

        // Thickness of the border around the MenuList overlay
        PropertyObserver<uint32_t>          borderThickness;

        // Height of each individual menu item
        PropertyObserver<uint32_t>          itemSize;

        // Color of the text of each item's label
        PropertyObserver<Color>             itemColor;

        // Adds the specified item to the list
        void addItem(const std::string& item);

        // Adds the specified items to the list
        void addItems(const std::initializer_list<std::string>& items);

        // Adds the specified items to the list
        void addItems(const std::vector<std::string>& items);

        // Removes the specified item from the list
        bool removeItem(const std::string& item);

        // Removes all items from the list
        void removeAllItems();

        // Returns the number of items in the combobox
        inline size_t getItemCount() const { return d_menuList->getItemCount(); }

    private:
        void _setupComboboxProperties();
        void _setupEventHandlers();

        Shared<MenuList>    d_menuList;
        std::string         d_selectedItem;
    };
}
