#include "TreeView2.h"
#include "visuals/RectVisual.h"

namespace mc {
    TreeViewNode::TreeViewNode() {
        _appendAllowedEvents();
    }

    TreeViewNode::TreeViewNode(
        const std::string& itemText,
        const std::string& key
    ) {
        this->itemText = itemText;
        this->key = key;

        _appendAllowedEvents();
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

    void TreeViewNode::removeChild(Shared<TreeViewNode> node) {
        removeChild(node->key);
    }

    void TreeViewNode::removeChild(const std::string& key) {
        size_t childIdx = d_nodeKeyIndexMap[key];
        auto& node = d_children[childIdx];

        d_children.erase(d_children.begin() + childIdx);
        _invalidateKeyIndexMap();

        node->off("nodeAdded");
        node->off("nodeRemoved");
        fireEvent("nodeRemoved", Event::empty);
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

        Size contentSize = Size(0, 0);

        _traverseTreeNodes(d_rootNode, d_rootNodeLevel, [this, &contentSize](TreeViewNode* node, int level) {
            auto& [btn, depthLevel] = d_nodeButtons[node];
            auto desiredSize = btn->getDesiredSizeWithMargins();

            const int32_t offset = 30;
            uint32_t offsetX = static_cast<uint32_t>(offset * (depthLevel - 1));
            
            contentSize.width += offsetX + desiredSize.width;
            contentSize.height += desiredSize.height;
        });

        return contentSize;
    }

    void TreeView2::_onArrangeChildren() {
        if (!d_rootNode || d_nodeButtons.empty()) {
            return;
        }

        Position pos = Position(0, 0);

        _traverseTreeNodes(d_rootNode, d_rootNodeLevel, [this, &pos](TreeViewNode* node, int level) {
            auto& [btn, depthLevel] = d_nodeButtons[node];
            auto size = btn->getDesiredSizeWithMargins();
            btn->setComputedSize(size);

            const int32_t offset = 30;
            btn->position->x = offset * (depthLevel - 1);
            btn->position->y = pos.y;
            pos.y += size.height;
        });
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

        cursorType = CursorType::Arrow;
    }

    void TreeView2::setRootNode(Shared<TreeViewNode> node) {
        if (d_rootNode) {
            _removeAllChildren();
        }

        d_rootNode = node;
        d_rootNode->on("nodeAdded", &TreeView2::_onTreeChanged, this);
        d_rootNode->on("nodeRemoved", &TreeView2::_onTreeChanged, this);
        d_rootNode->fireEvent("nodeAdded", Event::empty);
    }

    void TreeView2::_onTreeChanged(Shared<Event> e) {
        // Remove all children
        _removeAllChildren();
        d_nodeButtons.clear();

        _traverseTreeNodes(d_rootNode, d_rootNodeLevel, [this](TreeViewNode* node, int level) {
            printf("level: %i  item: %s\n", level, node->itemText.c_str());

            auto btn = MakeRef<Button>();
            btn->cornerRadius = 0;
            btn->horizontalAlignment = HAFill;
            btn->borderColor = Color::transparent;
            btn->backgroundColor = Color::transparent;
            btn->hoverOnColor = Color(120, 120, 120, 80);
            btn->mousePressedColor = Color(160, 160, 160, 80);
            btn->label->alignment = "left";
            btn->label->verticalPadding = 4;
            btn->label->text = node->itemText;
            btn->zIndex = 1;
            d_nodeButtons[node] = { btn, level };
            _addChildOffline(btn);
        });

        _signalLayoutChanged();
        printf("\n");
    }

    void TreeView2::_traverseTreeNodes(
        Shared<TreeViewNode> root,
        int level,
        std::function<void(TreeViewNode*, int)> callback
    ) {
        if (root.get() != d_rootNode.get()) {
            callback(root.get(), level);
        }

        for (auto& child : root->getChildren()) {
            _traverseTreeNodes(child, level + 1, callback);
        }
    }
} // namespace mc
