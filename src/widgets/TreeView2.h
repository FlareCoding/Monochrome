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

    PropertyObserver<std::string>   itemText;
    PropertyObserver<std::string>   key;
    PropertyObserver<bool>          expanded;

    //
    // TO-DO: explain in docs that the below functions apply
    //        only to DIRECT children on the current node level.
    //

    void addChild(Shared<TreeViewNode> node);
    void addChild(const std::string& itemText, const std::string& key = "");

    void insertChildBefore(Shared<TreeViewNode> node, Shared<TreeViewNode> beforeChild);
    void insertChildAfter(Shared<TreeViewNode> node, Shared<TreeViewNode> afterChild);

    bool removeChild(Shared<TreeViewNode> node);
    bool removeChild(const std::string& key);

    bool hasChildWithKey(const std::string& key);
    Shared<TreeViewNode> getChildWithKey(const std::string& key);
    Shared<TreeViewNode> findChildByKey(const std::string& key);

    Shared<TreeViewNode> firstChild();
    Shared<TreeViewNode> lastChild();

    inline std::vector<Shared<TreeViewNode>>& getChildren() {
        return d_children;
    }

    inline bool hasChildren() const { return !d_children.empty(); }

private:
    void _appendAllowedEvents();
    void _setupProperties();

    void _invalidateKeyIndexMap();

    std::vector<Shared<TreeViewNode>> d_children;
    std::map<std::string, size_t> d_nodeKeyIndexMap;
};

class TreeView2 : public BaseWidget {
public:
    TreeView2();

    std::string getWidgetName() const override { return "TreeView2"; }

    // @brief Background color of the empty space in the panel
    PropertyObserver<Color>         backgroundColor;

    PropertyObserver<Color>         itemHighlightedBorderColor;

    PropertyObserver<Color>         itemTextColor;

    PropertyObserver<std::string>   itemFont;

    PropertyObserver<uint32_t>      itemFontSize;

    bool allowParentNodeSelection = false;

    void setRootNode(Shared<TreeViewNode> node);

    void selectNode(Shared<TreeViewNode> node);
    void selectNodeByKey(const std::string& key);

    void expandNode(Shared<TreeViewNode> node);
    void expandNodeByKey(const std::string& key);

    void collapseNode(Shared<TreeViewNode> node);
    void collapseNodeByKey(const std::string& key);

    void toggleExpandNode(Shared<TreeViewNode> node);
    void toggleExpandNodeByKey(const std::string& key);

    bool hasNodeWithKey(const std::string& key);
    Shared<TreeViewNode> findNodeWithKey(const std::string& key);
    bool removeNodeWithKey(const std::string& key);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    Size _measureNodeButton(Shared<TreeViewNode> root);
    void _arrangeNodeButton(Shared<TreeViewNode> root, Position& availablePos);

    void _onTreeChanged(Shared<Event> e);

    void _traverseTreeNodes(
        Shared<TreeViewNode> root,
        int level,
        std::function<void(TreeViewNode*, int)> callback
    );

    void _nodeButtonOnClick(Shared<Event> e);

    void _selectNode(TreeViewNode* node);
    bool _hasNodeWithKey(Shared<TreeViewNode> root, const std::string& key);
    Shared<TreeViewNode> _findNodeWithKey(Shared<TreeViewNode> root, const std::string& key);
    bool _removeNodeWithKey(Shared<TreeViewNode> root, const std::string& key);

private:
    Shared<TreeViewNode> d_rootNode;
    const int d_rootNodeLevel = 0;

    std::map<TreeViewNode*, std::pair<Shared<Button>, int>> d_nodeButtons;
    std::map<Button*, TreeViewNode*> d_buttonToNodeMap;

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
