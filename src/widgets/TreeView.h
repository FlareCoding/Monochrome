#pragma once
#include "TreeViewGroup.h"

namespace mc {
class TreeView : public BaseWidget {
public:
    TreeView();

    // @brief Radius of the panel's corners
    PropertyObserver<uint32_t> cornerRadius;

    // @brief Background color of the empty space in the panel
    PropertyObserver<Color> backgroundColor;

    // @brief TO-DO
    PropertyObserver<Color> itemTextColor;

    // @brief TO-DO
    PropertyObserver<Color> itemBackgroundColor;

    // @brief TO-DO
    PropertyObserver<uint32_t> itemSize;

    void addGroup(Shared<TreeViewGroup> group);
    void removeGroup(const std::string& name);
    void removeGroup(Shared<TreeViewGroup> group);
    Shared<TreeViewGroup> getGroup(const std::string& name);

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
