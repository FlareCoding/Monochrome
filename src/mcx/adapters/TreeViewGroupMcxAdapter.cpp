#include "TreeViewGroupMcxAdapter.h"
#include <widgets/TreeViewGroup.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> TreeViewGroupMcxAdapter::createWidgetInstance() {
        return MakeRef<TreeViewGroup>();
    }

    void TreeViewGroupMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // Handle child mcx nodes manually as items to add
        mcxNode->childrenHandled = true;

        auto treeViewGroup = std::static_pointer_cast<TreeViewGroup>(widget);
        treeViewGroup->name =
            mcxNode->getAttribute("name", treeViewGroup->name);

        // Handle child nodes as items to be added
        for (auto& childNode : mcxNode->getChildren()) {
            auto nodeType = childNode->getType();

            // Sanity checking the node type
            if (nodeType != "item" && nodeType != "TreeViewGroup") {
                printf("Error parsing TreeViewGroup child node,"
                       "type has to be either <item> or <TreeViewGroup>\n");
                continue;
            }

            if (nodeType == "item") {
                // Making sure the node has a value attribute
                if (!childNode->hasAttribute("value")) {
                    printf("Error parsing TreeViewGroup"
                           "item node: expected <item value=\"...\"/>\n");
                    continue;
                }

                auto item = childNode->getAttribute("value");
                treeViewGroup->addItem(item);
            } else {
                // Parse the subgroup TreeViewGroup node
                auto subGroup = std::static_pointer_cast<TreeViewGroup>(
                    McxEngine::parseWidget(childNode)
                );

                treeViewGroup->addSubGroup(subGroup);
            }
        }
    }
} //namespace mc::mcx
