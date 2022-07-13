#pragma once
#include "BaseWidget.h"
#include "Panel.h"
#include "Button.h"
#include <variant>

namespace mc
{
    class Overlay;

    using MenuItem = std::string;

    class MenuList : public BaseWidget
    {
    public:
        MenuList();
        MenuList(const std::string& name);
        inline const std::string getType() const override { return "MenuList"; }

        void addMenu(Shared<MenuList> menu);
        void addMenuItem(const MenuItem& item);

        void setActivatorWidget(Shared<BaseWidget> widget);

        // If this MenuList is added as a child menu 
        // of another MenuList, then this is the name
        // that will be displayed on the menu button.
        PropertyObserver<std::string>   name;

        // Color of the border around the MenuList overlay
        PropertyObserver<Color>         borderColor;

        // Thickness of the border around the MenuList overlay
        PropertyObserver<uint32_t>      borderThickness;

        // Height of each individual menu item
        PropertyObserver<uint32_t>      menuItemSize;

    private:
        void _setupProperties() override;

        void _recalculateMenuItemBounds();

    private:
        Shared<Overlay> d_overlay;
        Shared<Panel> d_overlayBorder;
        Shared<Panel> d_contentPanel;

        std::map<
            std::string,
            std::pair<
                Shared<Button>,
                std::variant<Shared<MenuList>, MenuItem>
            >
        > d_menuItems;
    };
}
