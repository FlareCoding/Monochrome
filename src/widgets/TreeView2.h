#pragma once
#include "Button.h"

namespace mc {
class TreeViewNode {
public:
    std::string itemText;
    std::string key;

    //
    // TO-DO: explain in docs that the below functions apply
    //        only to DIRECT children on the current node level.
    //

    void addChild(Shared<TreeViewNode> node);
    void addChild(const std::string& itemText, const std::string& key = "");
    void insertChild(Shared<TreeViewNode> node, Shared<TreeViewNode> beforeChild);

    void removeChild(Shared<TreeViewNode> node);
    void removeChild(const std::string& key);

    bool hasChildWithKey(const std::string& key);

    inline std::vector<Shared<TreeViewNode>>& getChildren() { return d_children; }

private:
    std::vector<Shared<TreeViewNode>> d_children;
};

class TreeView2 : public BaseWidget {
public:
    TreeView2();

    std::string getWidgetName() const override { return "TreeView2"; }

    // @brief Background color of the empty space in the panel
    PropertyObserver<Color> backgroundColor;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();
};
} // namespace mc
