#include "DropdownButton.h"

namespace mc
{
    DropdownButton::DropdownButton() : Button() {}
    
    void DropdownButton::setMenuList(Shared<MenuList> menuList) {
        // If a previous menu list already exists,
        // remove this button from being an
        // activator for that menu list.
        if (d_menuList) {
            d_menuList->setActivatorWidget(nullptr);
        }

        d_menuList = menuList;
        d_menuList->setActivatorWidget(this);
    }

    Shared<MenuList> DropdownButton::getMenuList() {
        return d_menuList;
    }
}
