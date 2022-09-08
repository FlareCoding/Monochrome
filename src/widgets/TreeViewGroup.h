#pragma once
#include "StackPanel.h"
#include "Button.h"

namespace mc {
using TreeViewItem = std::string;

class TreeViewGroup : public StackPanel {
friend class TreeView;

public:
    TreeViewGroup();
    explicit TreeViewGroup(const std::string& name);

    PropertyObserver<std::string> name;

    void addSubGroup(Shared<TreeViewGroup> subGroup);
    void addItem(const TreeViewItem& item);

private:
    void _setupProperties();

    void _onGroupNameChanged(Shared<Event> e);
    void _expandButtonOnClick(Shared<Event> e);
    void _onItemClicked(Shared<Event> e);
    void _onItemLostFocus(Shared<Event> e);

    bool d_groupOpened = true;
    Shared<Button> d_expandGroupButton;
};
} // namespace mc
