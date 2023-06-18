#pragma once
#include "Button.h"
#include "MenuList.h"

namespace mc {
class DropdownButton : public Button {
public:
    DropdownButton();

    std::string getWidgetName() const override { return "DropdownButton"; }

    void setMenuList(Shared<MenuList> menuList);
    Shared<MenuList> getMenuList();

private:
    Shared<MenuList> d_menuList;
};
} // namespace mc
