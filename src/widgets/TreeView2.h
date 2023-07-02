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

    std::string itemText;
    std::string key;

    //
    // TO-DO: explain in docs that the below functions apply
    //        only to DIRECT children on the current node level.
    //

    void addChild(Shared<TreeViewNode> node);
    void addChild(const std::string& itemText, const std::string& key = "");

    void insertChildBefore(Shared<TreeViewNode> node, Shared<TreeViewNode> beforeChild);
    void insertChildAfter(Shared<TreeViewNode> node, Shared<TreeViewNode> afterChild);

    void removeChild(Shared<TreeViewNode> node);
    void removeChild(const std::string& key);

    bool hasChildWithKey(const std::string& key);
    Shared<TreeViewNode> getChildWithKey(const std::string& key);
    Shared<TreeViewNode> findChildByKey(const std::string& key);

    Shared<TreeViewNode> firstChild();
    Shared<TreeViewNode> lastChild();

    inline std::vector<Shared<TreeViewNode>>& getChildren() {
        return d_children;
    }

private:
    void _appendAllowedEvents();
    void _invalidateKeyIndexMap();

    std::vector<Shared<TreeViewNode>> d_children;
    std::map<std::string, size_t> d_nodeKeyIndexMap;
};

class TreeView2 : public BaseWidget {
public:
    TreeView2();

    std::string getWidgetName() const override { return "TreeView2"; }

    // @brief Background color of the empty space in the panel
    PropertyObserver<Color> backgroundColor;

    void setRootNode(Shared<TreeViewNode> node);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    void _onTreeChanged(Shared<Event> e);

    void _traverseTreeNodes(
        Shared<TreeViewNode> root,
        int level,
        std::function<void(TreeViewNode*, int)> callback
    );

private:
    Shared<TreeViewNode> d_rootNode;
    const int d_rootNodeLevel = 0;

    std::map<TreeViewNode*, std::pair<Shared<Button>, int>> d_nodeButtons;

private:
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
