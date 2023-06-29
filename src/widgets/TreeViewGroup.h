#pragma once
#include "StackPanel.h"
#include "Button.h"

namespace mc {
struct TreeViewItem {
    std::string name;
    std::string key;

    TreeViewItem() = default;

    TreeViewItem(
        const std::string& name,
        const std::string& key
    ) : name(name), key(key) {}

    bool operator==(const TreeViewItem& item) {
        return (this->key == item.key);
    }

    bool operator<(const TreeViewItem& item) const {
        return (this->key.c_str() < item.key.c_str());
    }
};

class TreeViewGroup : public StackPanel {
friend class TreeView;

public:
    TreeViewGroup();
    explicit TreeViewGroup(const std::string& name);

    std::string getWidgetName() const override { return "TreeViewGroup"; }

    // Text that gets displayed in the tree view
    PropertyObserver<std::string> name;

    // Unique ID specific to the tree view
    std::string key;

    void addSubGroup(Shared<TreeViewGroup> subGroup);
    void addItem(const TreeViewItem& item);
    void addItem(const std::string& name, const std::string& key = "");

    // Removes a node that matches the given key.
    // @returns true if the node was successfully
    // found and removed, false otherwise.
    bool removeNodeByKey(const std::string& key);

    // @returns Shared pointer to the TreeViewGroup associated with the given key
    Shared<TreeViewGroup> findGroupByKey(const std::string& key);

    // @returns Shared pointer to the TreeViewGroup associated with the given key
    TreeViewItem findItemByKey(const std::string& key);

private:
    void _setupProperties();

    void _onGroupNameChanged(Shared<Event> e);
    void _expandButtonOnClick(Shared<Event> e);
    void _onItemClicked(Shared<Event> e);
    void _onItemLostFocus(Shared<Event> e);

    bool d_groupOpened = true;
    Shared<Button> d_expandGroupButton;

    // { name, key } -> button associated with the item
    std::map<TreeViewItem, Shared<Button>> d_items;

    // { name, key } -> treeview group
    std::map<TreeViewItem, Shared<TreeViewGroup>> d_subGroups;

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
