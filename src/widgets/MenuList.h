#pragma once
#include "BaseWidget.h"
#include "Panel.h"
#include <variant>

namespace mc
{
    class Overlay;

    class MenuItem
    {
    public:
        std::string name;
        std::function<void()> selectionHandler;

        MenuItem() = default;
        MenuItem(const std::string& name) : name(name) {}
    };

    class MenuList : public BaseWidget
    {
    public:
        MenuList();
        MenuList(const std::string& name);
        inline const std::string getType() const override { return "MenuList"; }

        void addMenu(Shared<MenuList> menu);
        void addMenuItem(const MenuItem& item);

        void setActivatorWidget(Shared<BaseWidget> widget);

        PropertyObserver<std::string> name;

    private:
        void _setupProperties() override;

    private:
        Shared<Overlay> d_overlay;
        Shared<Panel> d_contentPanel;
        std::map<std::string, std::variant<Shared<MenuList>, MenuItem>> d_menuItems;

    private:
        const uint32_t  d_itemSize = 24u;
    };
}
