#pragma once
#include "Button.h"
#include "StackPanel.h"
#include <window/Overlay.h>
#include <variant>

namespace mc {
    using MenuItem = std::string;

    class MenuList : public BaseWidget {
    public:
        MenuList();
        explicit MenuList(const std::string& name);

        std::string getWidgetName() const override { return "MenuList"; }

        // Force shows the menu list overlay
        // window if no activator widget is set.
        void spawn(const Point& anchor = Point(50, 50));

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
        void setActivatorWidget(BaseWidget* widget, OverlayActivationType activationType);

        // If this MenuList is added as a child menu
        // of another MenuList, then this is the name
        // that will be displayed on the menu button.
        PropertyObserver<std::string>       name;

        // Color of the border around the MenuList overlay
        PropertyObserver<Color>             borderColor;

        // Thickness of the border around the MenuList overlay
        PropertyObserver<uint32_t>          borderThickness;

        // Specifies the preferred orientation for spawning
        // the overlay window around the activator widget.
        PropertyObserver<OverflowDirection> spawnDirection;

        // Font size of the menu items that determines their size in the menu list
        PropertyObserver<uint32_t>          itemSize;

        // Background color of each menu item
        PropertyObserver<Color>             itemBackgroundColor;

        // Text color of each menu item
        PropertyObserver<Color>             itemTextColor;

    protected:
        // Inherited via BaseWidget
        Size _measureSize() override;
        void _onSetComputedSize(const Size& size) override;

    private:
        void _setupProperties();
        void _recalculateGeometry();
        void _recalculateStyles();

    private:
        Shared<Overlay>     d_overlay;
        Shared<StackPanel>  d_contentPanel;

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
