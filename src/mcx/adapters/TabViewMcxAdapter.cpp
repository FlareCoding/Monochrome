#include "TabViewMcxAdapter.h"
#include <widgets/TabView.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> TabViewMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<TabView>();
    }

    void TabViewMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto tabView = std::static_pointer_cast<TabView>(widget);

        mcxNode->setAttribute("tabColor", _toString(tabView->tabColor));
        mcxNode->setAttribute("tabActiveColor", _toString(tabView->tabActiveColor));
        mcxNode->setAttribute("tabBorderColor", _toString(tabView->tabBorderColor));
        mcxNode->setAttribute("tabTextColor", _toString(tabView->tabTextColor));
        mcxNode->setAttribute("tabFont", tabView->tabFont);
        mcxNode->setAttribute("tabFontSize", _toString(tabView->tabFontSize));
    }

    void TabViewMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // Handle child mcx nodes manually as groups to add
        mcxNode->childrenHandled = true;

        // Apply TabView specific properties
        auto tabView = std::static_pointer_cast<TabView>(widget);
        tabView->tabColor =
            mcxNode->getColorAttribute("tabColor", tabView->tabColor);

        tabView->tabActiveColor =
            mcxNode->getColorAttribute("tabActiveColor", tabView->tabActiveColor);

        tabView->tabBorderColor =
            mcxNode->getColorAttribute("tabBorderColor", tabView->tabBorderColor);

        tabView->tabTextColor =
            mcxNode->getColorAttribute("tabTextColor", tabView->tabTextColor);

        tabView->tabFont =
            mcxNode->getAttribute("tabFont", tabView->tabFont);

        tabView->tabFontSize =
            mcxNode->getUIntAttribute("tabFontSize", tabView->tabFontSize);

        // Handle child nodes as items to be added
        for (auto& childNode : mcxNode->getChildren()) {
            // Sanity checking the node
            if (childNode->getType() != "tab") {
                printf("Error parsing TabView tab node: expected <tab name=\"...\">\n");
                continue;
            }

            // Make sure the tab has a name
            auto tabName = childNode->getAttribute("name");
            if (tabName.empty()) {
                printf("Error parsing TabView tab node: tab must have a name\n");
                continue;
            }

            // Check if the tab is marked as closable
            auto isTabClosable = childNode->getBoolAttribute("closable", false);

            // Make sure that there is a widget to be parsed inside the tab
            auto tabChildren = childNode->getChildren();
            if (tabChildren.size() != 1) {
                printf("Error parsing TabView tab node: tab must have exactly one child\n");
                continue;
            }

            auto tabContentNode = tabChildren.at(0);
            auto tabContent = McxEngine::parseWidget(tabContentNode);

            tabView->addTab(tabName, tabContent, isTabClosable);
        }
    }

    std::vector<std::string> TabViewMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "tabColor", "tabActiveColor", "tabBorderColor",
            "tabTextColor", "tabFont", "tabFontSize"
        };

        return props;
    }
} //namespace mc::mcx
