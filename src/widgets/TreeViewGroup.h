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

    std::string getWidgetName() const override { return "TreeViewGroup"; }

    PropertyObserver<std::string> name;

    void addSubGroup(Shared<TreeViewGroup> subGroup);
    void addItem(const TreeViewItem& item);

    // Removes an item or a sub-group that matches the given name.
    // @returns true if the item or sub-group was
    // successfully found and removed, false otherwise.
    bool remove(const std::string& name);

    // @returns Shared pointer to the TreeViewGroup associated with the given name
    Shared<TreeViewGroup> getGroup(const std::string& name);

private:
    void _setupProperties();

    void _onGroupNameChanged(Shared<Event> e);
    void _expandButtonOnClick(Shared<Event> e);
    void _onItemClicked(Shared<Event> e);
    void _onItemLostFocus(Shared<Event> e);

    bool d_groupOpened = true;
    Shared<Button> d_expandGroupButton;

    std::map<std::string, Shared<Button>> d_items;
    std::map<std::string, Shared<TreeViewGroup>> d_subGroups;

private:
    const char downArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xbc),
        static_cast<char>(0x20),
        0
    };

    const char rightArrowUtf8Prefix[6] = {
        static_cast<char>(0x20),
        static_cast<char>(0xe2),
        static_cast<char>(0x96),
        static_cast<char>(0xba),
        static_cast<char>(0x20),
        0
    };
};
} // namespace mc
