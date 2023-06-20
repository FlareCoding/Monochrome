#pragma once
#include "TreeViewGroup.h"

namespace mc {
class TreeView : public BaseWidget {
public:
    TreeView();

    std::string getWidgetName() const override { return "TreeView"; }

    // @brief Radius of the panel's corners
    PropertyObserver<uint32_t> cornerRadius;

    // @brief Background color of the empty space in the panel
    PropertyObserver<Color> backgroundColor;

    // @brief Text color of each tree item or item group
    PropertyObserver<Color> itemTextColor;

    // @brief Background color of each tree item or item group
    PropertyObserver<Color> itemBackgroundColor;

    // @brief Size of each tree item or item group
    PropertyObserver<uint32_t> itemSize;

    void addGroup(Shared<TreeViewGroup> group);

    void removeGroup(const std::string& name);
    void removeGroup(Shared<TreeViewGroup> group);
    
    void removeGroupById(const std::string& id);
    
    Shared<TreeViewGroup> getGroup(const std::string& name);
    Shared<TreeViewGroup> getGroupById(const std::string& id);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

private:
    std::vector<Shared<TreeViewGroup>> d_groups;
};
} // namespace mc
