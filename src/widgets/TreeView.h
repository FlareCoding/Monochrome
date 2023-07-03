#pragma once
#include "Button.h"

namespace mc {
class TreeViewNode : public EventEmitter {
public:
    TreeViewNode();
    TreeViewNode(
        const std::string& itemText,
        const std::string& key = ""
    );

    /// @brief Text displayed on the node in the TreeView
    PropertyObserver<std::string>   itemText;

    /// @brief Unique string identifier belonging to the node in the TreeView
    PropertyObserver<std::string>   key;

    /// @brief Determined whether or not the node is expanded and children
    /// are shown in the TreeView.
    /// @note Only has an effect when the node has children
    PropertyObserver<bool>          expanded;

    /// @brief Adds a node to the list of child nodes
    /// @param node Node to add to the child nodes list
    void addChild(Shared<TreeViewNode> node);

    /// @brief Creates and adds a node to the list of child nodes
    /// @param itemText Text to be displayed on the child node
    /// @param key Key to be assigned to the child node
    void addChild(const std::string& itemText, const std::string& key = "");

    /// @brief Inserts a child node before another given child node
    /// @param node Child node to be added
    /// @param beforeChild The reference node before which to insert the child
    void insertChildBefore(Shared<TreeViewNode> node, Shared<TreeViewNode> beforeChild);

    /// @brief Inserts a child node after another given child node
    /// @param node Child node to be added
    /// @param afterChild The reference node after which to insert the child
    void insertChildAfter(Shared<TreeViewNode> node, Shared<TreeViewNode> afterChild);

    /// @brief Removes a given node from the children's list
    /// @param node Reference to the node to be removed
    /// @returns true if node was removed successfully, false otherwise
    bool removeChild(Shared<TreeViewNode> node);

    /// @brief Removes a node with the given key from the children's list
    /// @param key Key of the node to be removed
    /// @returns true if node was removed successfully, false otherwise
    bool removeChild(const std::string& key);

    /// @brief Checks if there is a direct child node with the given key
    /// @param key Key of the child node
    /// @returns true if there is a direct child node with the given key, false otherwise
    bool hasChildWithKey(const std::string& key);

    /// @brief Assumes that the child node with the given key exists and attempts to return it
    /// @param key Key of the child node
    /// @returns Returns a reference to the child node with the given key.
    /// If no such node exists, an exception will be thrown.
    Shared<TreeViewNode> getChildWithKey(const std::string& key);

    /// @brief Checks if the child node with the given key exists and attempts to return it
    /// @param key Key of the child node
    /// @returns Returns a reference to the child node with
    /// the given key if such node exists, nullptr otherwise.
    Shared<TreeViewNode> findChildByKey(const std::string& key);

    /// @returns First child in the children's list if it exists, nullptr otherwise
    Shared<TreeViewNode> firstChild();

    /// @returns Last child in the children's list if it exists, nullptr otherwise
    Shared<TreeViewNode> lastChild();

    /// @returns Reference to the list of children nodes
    inline std::vector<Shared<TreeViewNode>>& getChildren() {
        return d_children;
    }

    /// @returns Whether or not this node has child nodes
    inline bool hasChildren() const { return !d_children.empty(); }

private:
    void _appendAllowedEvents();
    void _setupProperties();

    void _invalidateKeyIndexMap();

    /// @brief List of children nodes
    std::vector<Shared<TreeViewNode>> d_children;

    /// @brief Map associating node keys with their
    /// respective indices in the d_children list.
    std::map<std::string, size_t> d_nodeKeyIndexMap;
};

class TreeView : public BaseWidget {
public:
    TreeView();

    std::string getWidgetName() const override { return "TreeView"; }

    /// @brief Background color of the empty space in the panel
    PropertyObserver<Color>         backgroundColor;

    /// @brief Color of the border of the selected node's button
    PropertyObserver<Color>         itemHighlightedBorderColor;

    /// @brief Color of node buttons' text
    PropertyObserver<Color>         itemTextColor;

    /// @brief Node button's font
    PropertyObserver<std::string>   itemFont;

    /// @brief Node button's font size
    PropertyObserver<uint32_t>      itemFontSize;

    /// @brief Determines whether or not nodes that have children
    /// and can be expanded/collapsed can be selected themselves.
    bool allowParentNodeSelection = false;

    /// @brief Sets the root node of the tree.
    /// @note The top-most level root node has to be symbolic
    /// and contain actual main nodes as children because they
    /// are the ones that will get rendered.
    /// @param node Reference to the node that will be used as the tree's root
    void setRootNode(Shared<TreeViewNode> node);

    /// @returns Currently set root node
    inline Shared<TreeViewNode> getRootNode() { return d_rootNode; }

    /// @brief Selects and highlights a given node
    /// @param node Node to be selected
    void selectNode(Shared<TreeViewNode> node);

    /// @brief Selects and highlights a node with the given key
    /// @param key Key of the node to select
    void selectNodeByKey(const std::string& key);

    /// @brief Expands a parent node if possible
    /// @param node Node to be expanded
    void expandNode(Shared<TreeViewNode> node);

    /// @brief Expands a parent node with a given key if possible
    /// @param node Key to the node to be expanded
    void expandNodeByKey(const std::string& key);

    /// @brief Collapses a parent node if possible
    /// @param node Node to be collapsed
    void collapseNode(Shared<TreeViewNode> node);

    /// @brief Collapses a parent node with a given key if possible
    /// @param node Key to the node to be collapsed
    void collapseNodeByKey(const std::string& key);

    /// @brief Expands or collapses a parent node
    /// if possible depending on its current state.
    /// @param node Node to be expanded or collapsed
    void toggleExpandNode(Shared<TreeViewNode> node);

    /// @brief Expands or collapses a parent node with a
    /// given key if possible depending on its current state.
    /// @param node Key of the node to be expanded or collapsed
    void toggleExpandNodeByKey(const std::string& key);

    /// @brief Recursively checks if a node with a given key exists in the tree
    /// @param key Key of the target node
    /// @returns true if there is a child node in
    /// the tree with the given key, false otherwise.
    bool hasNodeWithKey(const std::string& key);

    /// @brief Attempts to recursively find a
    /// node with the given key in the tree.
    /// @param key Key of the target node
    /// @returns Reference to the target node with the
    /// given key, nullptr if such node could not be found.
    Shared<TreeViewNode> findNodeWithKey(const std::string& key);

    /// @brief Attempts to remove a node with the given key from the tree
    /// @param key Key of the node to remove
    /// @returns true if the node was successfully removed, false otherwise
    bool removeNodeWithKey(const std::string& key);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    Size _measureNodeButton(Shared<TreeViewNode> root);
    void _arrangeNodeButton(Shared<TreeViewNode> root, Position& availablePos);

    /// @brief Reconstructs visual elements of the tree
    void _onTreeChanged(Shared<Event> e);

    /// @brief Recursively walks through the tree level by level
    /// @param root Current depth level's root node
    /// @param level Current depth level
    /// @param callback Callback function to be executed on each node
    void _traverseTreeNodes(
        Shared<TreeViewNode> root,
        int level,
        std::function<void(TreeViewNode*, int)> callback
    );

    void _nodeButtonOnClick(Shared<Event> e);

    /// @brief Internal implementation of selectNode
    void _selectNode(TreeViewNode* node);

    /// @brief Internal recursive implementation of hasNodeWithKey
    bool _hasNodeWithKey(Shared<TreeViewNode> root, const std::string& key);

    /// @brief Internal recursive implementation of findNodeWithKey
    Shared<TreeViewNode> _findNodeWithKey(Shared<TreeViewNode> root, const std::string& key);

    /// @brief Internal recursive implementation of removeNodeWithKey
    bool _removeNodeWithKey(Shared<TreeViewNode> root, const std::string& key);

private:
    Shared<TreeViewNode> d_rootNode;
    const int d_rootNodeLevel = 0;

    /// @brief Node --> { node's button, depth level }
    std::map<TreeViewNode*, std::pair<Shared<Button>, int>> d_nodeButtons;

    /// @brief Node's button --> associated node
    std::map<Button*, TreeViewNode*> d_buttonToNodeMap;

    /// @brief Currently selected node (nullptr if no node is selected)
    TreeViewNode* d_selectedNode = nullptr;

private:
    const int32_t d_nodeDepthLevelOffset = 30;

    const char d_downArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xbc),
        static_cast<char>(0x20),
        0
    };

    const char d_rightArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xba),
        static_cast<char>(0x20),
        0
    };
};
} // namespace mc
