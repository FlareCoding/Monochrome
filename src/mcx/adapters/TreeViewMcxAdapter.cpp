#include "TreeViewMcxAdapter.h"
#include <widgets/TreeView.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> TreeViewMcxAdapter::createWidgetInstance() {
        return MakeRef<TreeView>();
    }

    void TreeViewMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // Handle child mcx nodes manually as groups to add
        mcxNode->childrenHandled = true;

        // Apply TreeView specific properties
        auto treeView = std::static_pointer_cast<TreeView>(widget);
        treeView->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", treeView->cornerRadius);

        treeView->itemSize =
            mcxNode->getUIntAttribute("itemSize", treeView->itemSize);

        treeView->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", treeView->backgroundColor);

        treeView->itemTextColor =
            mcxNode->getColorAttribute("itemTextColor", treeView->itemTextColor);

        treeView->itemBackgroundColor =
            mcxNode->getColorAttribute("itemBackground", treeView->itemBackgroundColor);

        // Handle child nodes as items to be added
        for (auto& childNode : mcxNode->getChildren()) {
            // Sanity checking the node
            if (childNode->getType() != "TreeViewGroup") {
                printf("Error parsing TreeView item node: expected <TreeViewGroup>\n");
                continue;
            }

            auto group = std::static_pointer_cast<TreeViewGroup>(
                McxEngine::parseWidget(childNode)
            );

            treeView->addGroup(group);
        }
    }
} //namespace mc::mcx
