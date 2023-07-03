#include "TreeViewMcxAdapter.h"
#include <widgets/TreeView.h>
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> TreeViewMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<TreeView>();
    }

    void TreeViewMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto treeView = std::static_pointer_cast<TreeView>(widget);

        mcxNode->setAttribute("backgroundColor", _toString(treeView->backgroundColor));
        mcxNode->setAttribute("itemTextColor", _toString(treeView->itemTextColor));
        mcxNode->setAttribute("itemFont", treeView->itemFont);
        mcxNode->setAttribute("itemFontSize", _toString(treeView->itemFontSize));

        mcxNode->setAttribute("itemHighlightedBorderColor",
            _toString(treeView->itemHighlightedBorderColor));

        mcxNode->setAttribute("allowParentNodeSelection",
            _toString(treeView->allowParentNodeSelection));
    }

    void TreeViewMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        // Handle child mcx nodes manually as items to add
        mcxNode->childrenHandled = true;

        // Apply TreeView specific properties
        auto treeView = std::static_pointer_cast<TreeView>(widget);

        treeView->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", treeView->backgroundColor);

        treeView->itemTextColor =
            mcxNode->getColorAttribute("itemTextColor", treeView->itemTextColor);

        treeView->itemHighlightedBorderColor =
            mcxNode->getColorAttribute("itemHighlightedBorderColor",
                                        treeView->itemHighlightedBorderColor);

        treeView->itemFont = mcxNode->getAttribute("itemFont", treeView->itemFont);

        treeView->itemFontSize =
            mcxNode->getUIntAttribute("itemFontSize", treeView->itemFontSize);

        treeView->allowParentNodeSelection =
            mcxNode->getBoolAttribute("allowParentNodeSelection",
                                       treeView->allowParentNodeSelection);

        auto rootNode = MakeRef<TreeViewNode>("", "__root");
        treeView->setRootNode(rootNode);

        // Handle children nodes as items to be added
        for (auto& childMcxNode : mcxNode->getChildren()) {
            auto treeNode = _parseTreeViewNodeMcxNode(childMcxNode);

            if (treeNode) {
                rootNode->addChild(treeNode);
            }
        }
    }

    std::vector<std::string> TreeViewMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "backgroundColor", "itemTextColor", "itemFont",
            "itemFontSize", "itemHighlightedBorderColor",
            "allowParentNodeSelection"
        };

        return props;
    }

    Shared<TreeViewNode> TreeViewMcxAdapter::_parseTreeViewNodeMcxNode(Shared<McxNode>& mcxNode) {
        auto nodeType = mcxNode->getType();

        // Sanity checking the node type
        if (nodeType != "item") {
            printf("Error parsing TreeView child node, "
                   "type has to be <item>\n");

            return nullptr;
        }

        auto nodeText = mcxNode->getAttribute("name");
        auto nodeKey = mcxNode->getAttribute("key");

        auto treeNode = MakeRef<TreeViewNode>(nodeText, nodeKey);

        for (auto& childMcxNode : mcxNode->getChildren()) {
            auto childTreeNode = _parseTreeViewNodeMcxNode(childMcxNode);

            if (childTreeNode) {
                treeNode->addChild(childTreeNode);
            }
        }

        if (mcxNode->getBoolAttribute("expanded")) {
            if (!treeNode->hasChildren()) {
                printf("Error expanding TreeView child node "
                       "'%s', node has no children\n", treeNode->itemText->c_str());
                return treeNode;
            }

            treeNode->expanded = true;
        }

        return treeNode;
    }
} // namespace mc::mcx
