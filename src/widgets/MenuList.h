#pragma once
#include "Panel.h"
#include "Button.h"
#include <variant>

namespace mc {
class Overlay;

using MenuItem = std::string;

class MenuList : public BaseWidget {
public:
    MenuList();
    explicit MenuList(const std::string& name);
    inline const std::string getType() const override { return "MenuList"; }

    // Adds another MenuList as a submenu that will
    // open when it's selection button is clicked.
    void addSubMenu(Shared<MenuList> menu);

    // Adds a menu item button to the items list.
    void addMenuItem(const MenuItem& item);

    // Removes a menu item or a submenu
    // that matches the specified name.
    // @return true if the item was successfully removed,
    //         false if the item wasn't found.
    bool removeItem(const std::string& name);

    // Removes all menu items and submenus in the current MenuList
    void removeAllItems();

    // Returns the number of items (submenus included)
    inline size_t getItemCount() const { return d_menuItems.size(); }

    // Returns the index of the specified item or submenu name
    size_t indexOf(const std::string& name);

    // Returns the name of the item at the given index
    std::string getItemName(size_t index);

    // Gives the ability to the specified widget
    // to open this MenuList on the click event.
    void setActivatorWidget(BaseWidget* widget);

    // If this MenuList is added as a child menu
    // of another MenuList, then this is the name
    // that will be displayed on the menu button.
    PropertyObserver<std::string>       name;

    // Color of the border around the MenuList overlay
    PropertyObserver<Color>             borderColor;

    // Thickness of the border around the MenuList overlay
    PropertyObserver<uint32_t>          borderThickness;

    // Height of each individual menu item
    PropertyObserver<uint32_t>          menuItemSize;

    // Specifies the preferred orientation for spawning
    // the overlay window around the activator widget.
    PropertyObserver<OverflowDirection> spawnDirection;

private:
    void _setupProperties() override;

    void _recalculateMenuItemBounds();

private:
    Shared<Overlay> d_overlay;
    Shared<Panel> d_overlayBorder;
    Shared<Panel> d_contentPanel;

    std::vector<
        std::pair<
            std::string,
            std::pair<
                Shared<Button>,
                std::variant<Shared<MenuList>, MenuItem>
            >
        >
    > d_menuItems;
};
} // namespace mc
