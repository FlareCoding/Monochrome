#include "TreeView2.h"
#include "visuals/RectVisual.h"

namespace mc {
    TreeViewNode::TreeViewNode() {
        _appendAllowedEvents();
        _setupProperties();
    }

    TreeViewNode::TreeViewNode(
        const std::string& itemText,
        const std::string& key
    ) {
        this->itemText = itemText;
        this->key = key;

        _appendAllowedEvents();
        _setupProperties();
    }

    void TreeViewNode::addChild(Shared<TreeViewNode> node) {
        insertChildAfter(node, lastChild());
    }

    void TreeViewNode::addChild(const std::string& itemText, const std::string& key) {
        addChild(MakeRef<TreeViewNode>(itemText, key));    
    }
    
    void TreeViewNode::insertChildBefore(
        Shared<TreeViewNode> node,
        Shared<TreeViewNode> beforeChild
    ) {
        size_t referenceChildIdx = beforeChild ? d_nodeKeyIndexMap[beforeChild->key] : 0;
        
        d_children.insert(d_children.begin() + referenceChildIdx, node);
        _invalidateKeyIndexMap();

        node->forwardEmittedEvents(this);
        fireEvent("nodeRemoved", Event::empty);
    }

    void TreeViewNode::insertChildAfter(
        Shared<TreeViewNode> node,
        Shared<TreeViewNode> afterChild
    ) {
        size_t referenceChildIdx = (afterChild ? d_nodeKeyIndexMap[afterChild->key] + 1 : 0);
        
        d_children.insert(d_children.begin() + referenceChildIdx, node);
        _invalidateKeyIndexMap();

        node->forwardEmittedEvents(this);
        fireEvent("nodeRemoved", Event::empty);
    }

    bool TreeViewNode::removeChild(Shared<TreeViewNode> node) {
        return removeChild(node->key);
    }

    bool TreeViewNode::removeChild(const std::string& key) {
        if (!hasChildWithKey(key)) {
            return false;
        }

        size_t childIdx = d_nodeKeyIndexMap[key];
        auto& node = d_children[childIdx];

        d_children.erase(d_children.begin() + childIdx);
        _invalidateKeyIndexMap();

        node->off("nodeAdded");
        node->off("nodeRemoved");
        fireEvent("nodeRemoved", Event::empty);

        return true;
    }

    bool TreeViewNode::hasChildWithKey(const std::string& key) {
        return d_nodeKeyIndexMap.find(key) != d_nodeKeyIndexMap.end();
    }

    Shared<TreeViewNode> TreeViewNode::getChildWithKey(const std::string& key) {
        size_t childIdx = d_nodeKeyIndexMap[key];
        return d_children[childIdx];
    }

    Shared<TreeViewNode> TreeViewNode::findChildByKey(const std::string& key) {
        if (hasChildWithKey(key)) {
            return getChildWithKey(key);
        }

        return nullptr;
    }

    Shared<TreeViewNode> TreeViewNode::firstChild() {
        if (d_children.empty()) {
            return nullptr;
        }

        return d_children.front();
    }

    Shared<TreeViewNode> TreeViewNode::lastChild() {
        if (d_children.empty()) {
            return nullptr;
        }

        return d_children.back();
    }

    void TreeViewNode::_appendAllowedEvents() {
        appendAllowedEvent("nodeAdded");
        appendAllowedEvent("nodeRemoved");
        appendAllowedEvent("nodeChanged");
        appendAllowedEvent("nodeExpanded");
        appendAllowedEvent("nodeCollapsed");
    }

    void TreeViewNode::_setupProperties() {
        itemText.forwardEmittedEvents(this);
        key.forwardEmittedEvents(this);

        expanded = false;
        expanded.on("propertyChanged", [this](Shared<Event> e) {
            if (!hasChildren()) {
                e->stopPropagation();
                return;
            }

            if (expanded.get()) {
                fireEvent("nodeExpanded", Event::empty);
            } else {
                fireEvent("nodeCollapsed", Event::empty);
            }
        });
        expanded.forwardEmittedEvents(this);
    }

    void TreeViewNode::_invalidateKeyIndexMap() {
        d_nodeKeyIndexMap.clear();

        for (size_t i = 0; i < d_children.size(); ++i) {
            d_nodeKeyIndexMap[d_children.at(i)->key] = i;
        }
    }

    TreeView2::TreeView2() {
        appendAllowedEvent("itemSelected");

        _createVisuals();
        _setupProperties();
    }

    Size TreeView2::_measureSize() {
        if (!d_rootNode || d_nodeButtons.empty()) {
            return Size(0, 0);
        }

        return _measureNodeButton(d_rootNode);
    }

    void TreeView2::_onArrangeChildren() {
        if (!d_rootNode || d_nodeButtons.empty()) {
            return;
        }

        auto initialPosition = Position(0, 0);
        _arrangeNodeButton(d_rootNode, initialPosition);
    }

    Size TreeView2::_measureNodeButton(Shared<TreeViewNode> root) {
        Size nodeSize = Size(0, 0);

        // The root node is a symbolic node that holds actual main nodes under one parent
        if (root.get() != d_rootNode.get()) {
            auto& [nodeButton, depthLevel] = d_nodeButtons.at(root.get());
            auto nodeButtonDesiredSize = nodeButton->getDesiredSizeWithMargins();

            uint32_t offsetX = static_cast<uint32_t>(d_nodeDepthLevelOffset * (depthLevel - 1));
            
            if (nodeSize.width < offsetX + nodeButtonDesiredSize.width) {
                nodeSize.width = offsetX + nodeButtonDesiredSize.width;
            }

            nodeSize.height += nodeButtonDesiredSize.height;
        }

        // Measure children
        if (root->hasChildren() && root->expanded.get()) {
            for (auto& child : root->getChildren()) {
                Size childSize = _measureNodeButton(child);
                nodeSize.height += childSize.height;

                if (nodeSize.width < childSize.width) {
                    nodeSize.width = childSize.width;
                }
            }
        }

        return nodeSize;
    }

    void TreeView2::_arrangeNodeButton(
        Shared<TreeViewNode> root,
        Position& availablePos
    ) {
        // The root node is a symbolic node that holds actual main nodes under one parent
        if (root.get() != d_rootNode.get()) {
            auto contentSize = getComputedSize();
            auto& [nodeButton, depthLevel] = d_nodeButtons.at(root.get());

            auto nodeButtonDesiredSize = nodeButton->getDesiredSizeWithMargins();
            nodeButtonDesiredSize.width = contentSize.width;
            nodeButton->setComputedSize(nodeButtonDesiredSize);

            nodeButton->position->x = d_nodeDepthLevelOffset * (depthLevel - 1);
            nodeButton->position->y = availablePos.y;
            availablePos.y += nodeButtonDesiredSize.height;
        }

        // Arrange children
        if (root->hasChildren() && root->expanded.get()) {
            for (auto& child : root->getChildren()) {
                _arrangeNodeButton(child, availablePos);
            }
        }
    }

    void TreeView2::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void TreeView2::_setupProperties() {
        backgroundColor = Color::transparent;
        backgroundColor.forwardEmittedEvents(this);

        itemHighlightedBorderColor = Color::white;
        itemHighlightedBorderColor.forwardEmittedEvents(this);

        itemTextColor = Color::white;
        itemTextColor.on("propertyChanged", &TreeView2::_onTreeChanged, this);

        itemFont = "Arial";
        itemFont.on("propertyChanged", &TreeView2::_onTreeChanged, this);

        itemFontSize = 12;
        itemFontSize.on("propertyChanged", &TreeView2::_onTreeChanged, this);

        cursorType = CursorType::Hand;
    }

    void TreeView2::setRootNode(Shared<TreeViewNode> node) {
        if (d_rootNode) {
            _removeAllChildren();
        }

        node->expanded = true;
        d_rootNode = node;

        d_rootNode->on("nodeAdded", &TreeView2::_onTreeChanged, this);
        d_rootNode->on("nodeRemoved", &TreeView2::_onTreeChanged, this);
        d_rootNode->on("nodeExpanded", &TreeView2::_onTreeChanged, this);
        d_rootNode->on("nodeCollapsed", &TreeView2::_onTreeChanged, this);
        d_rootNode->fireEvent("nodeAdded", Event::empty);
    }

    void TreeView2::selectNode(Shared<TreeViewNode> node) {
        _selectNode(node.get());
    }

    void TreeView2::selectNodeByKey(const std::string& key) {
        auto targetNode = findNodeWithKey(key);
        _selectNode(targetNode.get());
    }

    bool TreeView2::hasNodeWithKey(const std::string& key) {
        return _hasNodeWithKey(d_rootNode, key);
    }

    Shared<TreeViewNode> TreeView2::findNodeWithKey(const std::string& key) {
        return _findNodeWithKey(d_rootNode, key);
    }

    bool TreeView2::removeNodeWithKey(const std::string& key) {
        return _removeNodeWithKey(d_rootNode, key);
    }

    void TreeView2::expandNode(Shared<TreeViewNode> node) {
        if (!node || !node->hasChildren() || node->expanded.get()) {
            return;
        }

        node->expanded = true;
    }

    void TreeView2::expandNodeByKey(const std::string& key) {
        auto targetNode = findNodeWithKey(key);
        expandNode(targetNode);
    }

    void TreeView2::collapseNode(Shared<TreeViewNode> node) {
        if (!node || !node->hasChildren() || !node->expanded.get()) {
            return;
        }

        node->expanded = false;
    }

    void TreeView2::collapseNodeByKey(const std::string& key) {
        auto targetNode = findNodeWithKey(key);
        collapseNode(targetNode);
    }

    void TreeView2::toggleExpandNode(Shared<TreeViewNode> node) {
        if (!node || !node->hasChildren()) {
            return;
        }

        node->expanded = !node->expanded.get();
    }

    void TreeView2::toggleExpandNodeByKey(const std::string& key) {
        auto targetNode = findNodeWithKey(key);
        toggleExpandNode(targetNode);
    }

    void TreeView2::_onTreeChanged(Shared<Event> e) {
        // Remove all children
        d_nodeButtons.clear();
        d_buttonToNodeMap.clear();
        _removeAllChildren();

        _traverseTreeNodes(d_rootNode, d_rootNodeLevel, [this](TreeViewNode* node, int level) {
            auto btn = MakeRef<Button>();
            btn->cornerRadius = 0;
            btn->horizontalAlignment = HAFill;
            btn->borderColor = Color::transparent;
            btn->backgroundColor = Color::transparent;
            btn->hoverOnColor = Color(120, 120, 120, 80);
            btn->mousePressedColor = Color(160, 160, 160, 80);
            btn->label->alignment = "left";
            btn->label->verticalPadding = 4;
            btn->label->color = itemTextColor;
            btn->label->font = itemFont;
            btn->label->fontSize = itemFontSize;
            btn->zIndex = 1;
            btn->on("clicked", &TreeView2::_nodeButtonOnClick, this);

            if (node == d_selectedNode) {
                btn->backgroundColor = Color(160, 160, 160, 80);
                btn->borderColor = itemHighlightedBorderColor;
            }

            if (node->hasChildren()) {
                if (node->expanded.get()) {
                    btn->label->text = d_downArrowUtf8Prefix + node->itemText.get();
                } else {
                    btn->label->text = d_rightArrowUtf8Prefix + node->itemText.get();
                }
            } else {
                btn->label->text = node->itemText;
            }

            d_nodeButtons[node] = { btn, level };
            _addChildOffline(btn);

            d_buttonToNodeMap[btn.get()] = node;
        });

        _signalLayoutChanged();
        fireEvent("propertyChanged", Event::empty);
    }

    void TreeView2::_traverseTreeNodes(
        Shared<TreeViewNode> root,
        int level,
        std::function<void(TreeViewNode*, int)> callback
    ) {
        if (root.get() != d_rootNode.get()) {
            callback(root.get(), level);
        }

        // Don't iterate over an invalid root
        if (!root) {
            return;
        }

        for (auto& child : root->getChildren()) {
            _traverseTreeNodes(child, level + 1, callback);
        }
    }

    void TreeView2::_nodeButtonOnClick(Shared<Event> e) {
        // Prevent memory corruption bugs related
        // to widget focus when the tree changes.
        e->target->unfocus();

        auto selectedNodeButton = static_cast<Button*>(e->target);
        auto node = d_buttonToNodeMap.at(selectedNodeButton);

        if (!node->hasChildren() || allowParentNodeSelection) {
            _selectNode(node);
        } else {
            _selectNode(nullptr);
        }

        std::string groupAction = "none";

        // Expand/collapse the node
        if (node->hasChildren()) {
            node->expanded = !node->expanded.get();

            if (node->expanded.get()) {
                groupAction = "expanded";
            } else {
                groupAction = "collapsed";
            }
        }

        // Fire the event to indicate that an item has been selected
        fireEvent("itemSelected", {
            { "item", node->itemText.get() },
            { "key", node->key.get() },
            { "expanded", (node->hasChildren() && node->expanded.get()) },
            { "isLeaf", !node->hasChildren() },
            { "groupAction", groupAction }
        });

        // Force a redraw
        fireEvent("propertyChanged", Event::empty);
    }

    void TreeView2::_selectNode(TreeViewNode* node) {
        // The node is already selected
        if (node == d_selectedNode) {
            return;
        }

        // Remove highlight from the previously selected node's button
        if (d_selectedNode) {
            auto previouslySelectedNodeButton = d_nodeButtons.at(d_selectedNode).first;
            previouslySelectedNodeButton->backgroundColor = Color::transparent;
            previouslySelectedNodeButton->borderColor = Color::transparent;
        }

        // Check if there is no newly provided node to highlight
        if (!node) {
            d_selectedNode = nullptr;
            return;
        }

        // Get the button associated with the new node
        CORE_ASSERT(d_nodeButtons.find(node) != d_nodeButtons.end(),
            "The current TreeView doesn't have the provided node as its child: 0x" +
            std::to_string(reinterpret_cast<uint64_t>(node)));

        // Set the highlight on the newly selected node's button
        auto newSelectedNodeButton = d_nodeButtons.at(node).first;
        newSelectedNodeButton->backgroundColor = Color(160, 160, 160, 80);
        newSelectedNodeButton->borderColor = itemHighlightedBorderColor;

        // Update the reference to the selected node
        d_selectedNode = node;
    }

    bool TreeView2::_hasNodeWithKey(
        Shared<TreeViewNode> root,
        const std::string& key
    ) {
        if (!root) {
            return false;
        }

        // If the current node has the given key, return true
        if (root->key.get() == key) {
            return true;
        }

        // If the current node doesn't have the given key,
        // then one of it's children has to have it.
        // If there are no children, return false.
        if (!root->hasChildren()) {
            return false;
        }

        // Walk through every child and check if it fits the criteria 
        for (auto& child : root->getChildren()) {
            if (_hasNodeWithKey(child, key)) {
                return true;
            }
        }

        // No child with key was found
        return false;
    }

    Shared<TreeViewNode> TreeView2::_findNodeWithKey(
        Shared<TreeViewNode> root,
        const std::string& key
    ) {
        if (!root) {
            return nullptr;
        }

        // Check if the current node has the given key
        if (root->key.get() == key) {
            return root;
        }

        // If the current node doesn't match,
        // check if it has children.
        if (!root->hasChildren()) {
            return nullptr;
        }

        // Walk through every child and check if any fit the criteria
        for (auto& child : root->getChildren()) {
            auto targetNode = _findNodeWithKey(child, key);
            if (targetNode) {
                return targetNode;
            }
        }

        // No child with key was found
        return nullptr;
    }

    bool TreeView2::_removeNodeWithKey(
        Shared<TreeViewNode> root,
        const std::string& key
    ) {
        if (!root) {
            return false;
        }

        // If current node doesn't have children, return false
        if (!root->hasChildren()) {
            return false;
        }

        // If the current node has the given key, return true
        if (root->removeChild(key)) {
            return true;
        }

        // Walk through every child and check if any fit the criteria
        for (auto& child : root->getChildren()) {
            if (_removeNodeWithKey(child, key)) {
                return true;
            }
        }

        // No child with key was found
        return false;
    }
} // namespace mc
