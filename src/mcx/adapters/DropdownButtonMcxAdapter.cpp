#include "DropdownButtonMcxAdapter.h"
#include <widgets/DropdownButton.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    DropdownButtonMcxAdapter::DropdownButtonMcxAdapter() {
        d_buttonAdapter = MakeRef<ButtonMcxAdapter>();
    }

    Shared<BaseWidget> DropdownButtonMcxAdapter::createWidgetInstance() {
        return MakeRef<DropdownButton>();
    }

    void DropdownButtonMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // First apply button-specific properties
        // because DropdownButton is just a Button's subclass.
        d_buttonAdapter->applyProperties(widget, mcxNode);

        // Handle child mcx nodes manually as items to add
        mcxNode->childrenHandled = true;

        // Apply dropdown button specific properties
        auto dropdownButton = std::static_pointer_cast<DropdownButton>(widget);

        // Handle child menulist node
        auto& children = mcxNode->getChildren();
        if (!children.size()) {
            return;
        }

        if (children.size() > 1) {
            printf("DropdownButton must have only one MenuList child widget\n");
            return;
        }

        auto& childNode = children.at(0);

        // Sanity checking the node
        if (childNode->getType() != "MenuList") {
            printf("Error parsing DropdownButton, child node has to be of type MenuList\n");
            return;
        }

        auto menuList = std::static_pointer_cast<MenuList>(McxEngine::parseWidget(childNode));
        dropdownButton->setMenuList(menuList);
    }
} //namespace mc::mcx
