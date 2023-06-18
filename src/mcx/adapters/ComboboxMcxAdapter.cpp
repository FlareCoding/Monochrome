#include "ComboboxMcxAdapter.h"
#include <widgets/Combobox.h>

namespace mc::mcx {
    ComboboxMcxAdapter::ComboboxMcxAdapter() {
        d_buttonAdapter = MakeRef<ButtonMcxAdapter>();
    }

    Shared<BaseWidget> ComboboxMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<Combobox>();
    }

    void ComboboxMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // First apply button-specific properties
        // because Combobox is just a Button's subclass.
        d_buttonAdapter->applyProperties(widget, mcxNode);

        // Handle child mcx nodes manually as items to add
        mcxNode->childrenHandled = true;

        // Apply combobox specific properties
        auto combobox = std::static_pointer_cast<Combobox>(widget);

        combobox->overlayBorderColor =
            mcxNode->getColorAttribute("overlayBorderColor", combobox->overlayBorderColor);

        combobox->itemColor =
            mcxNode->getColorAttribute("itemColor", combobox->itemColor);

        combobox->itemBackgroundColor =
            mcxNode->getColorAttribute("itemBackgroundColor", combobox->itemBackgroundColor);

        combobox->overlayBorderThickness =
            mcxNode->getUIntAttribute("overlayBorderThickness", combobox->overlayBorderThickness);

        combobox->itemSize =
            mcxNode->getUIntAttribute("itemSize", combobox->itemSize);

        auto overlaySpawnDirection = mcxNode->getAttribute("overlaySpawnDirection", "down");
        if (overlaySpawnDirection == "left") {
            combobox->overlaySpawnDirection = OverflowDirection::Left;
        } else if (overlaySpawnDirection == "right") {
            combobox->overlaySpawnDirection = OverflowDirection::Right;
        } else if (overlaySpawnDirection == "up") {
            combobox->overlaySpawnDirection = OverflowDirection::Up;
        } else if (overlaySpawnDirection == "down") {
            combobox->overlaySpawnDirection = OverflowDirection::Down;
        }

        // Handle child nodes as items to be added
        for (auto& childNode : mcxNode->getChildren()) {
            // Sanity checking the node
            if (childNode->getType() != "item") {
                printf("Error parsing combobox item node: expected <item value=\"...\"/>\n");
                continue;
            }

            // Making sure the node has a value attribute
            if (!childNode->hasAttribute("value")) {
                printf("Error parsing combobox item node: expected <item value=\"...\"/>\n");
                continue;
            }

            auto itemValue = childNode->getAttribute("value");
            combobox->addItem(itemValue);
        }
    }
} //namespace mc::mcx
