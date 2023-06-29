#include "TreeViewGroup.h"

#define TREE_INDENT_MARGIN 30

namespace mc {
    TreeViewGroup::TreeViewGroup() {
        appendAllowedEvent("itemSelected");
        _setupProperties();
    }

    TreeViewGroup::TreeViewGroup(const std::string& name) {
        appendAllowedEvent("itemSelected");
        _setupProperties();

        this->name = name;
    }

    void TreeViewGroup::addSubGroup(Shared<TreeViewGroup> subGroup) {
        subGroup->marginLeft = TREE_INDENT_MARGIN;
        subGroup->zIndex = 1;
        subGroup->horizontalAlignment = HAFill;
        subGroup->forwardEmittedEvent(this, "itemSelected");
        addChild(subGroup);

        d_subGroups.insert({ TreeViewItem(subGroup->name.get(), subGroup->key), subGroup });
    }

    void TreeViewGroup::addItem(const TreeViewItem& item) {
        auto itemButton = MakeRef<Button>();
        itemButton->cornerRadius = 0;
        itemButton->horizontalAlignment = HAFill;
        itemButton->borderColor = Color::transparent;
        itemButton->backgroundColor = Color::transparent;
        itemButton->hoverOnColor = Color(120, 120, 120, 80);
        itemButton->mousePressedColor = Color(160, 160, 160, 80);
        itemButton->label->alignment = "left";
        itemButton->label->verticalPadding = 4;
        itemButton->label->text = item.name;
        itemButton->marginLeft = TREE_INDENT_MARGIN;
        itemButton->zIndex = 1;
        itemButton->on("clicked", &TreeViewGroup::_onItemClicked, this);
        itemButton->on("lostFocus", &TreeViewGroup::_onItemLostFocus, this);
        addChild(itemButton);

        d_items.insert({ TreeViewItem(item.name, item.key), itemButton });
    }

    void TreeViewGroup::addItem(const std::string& name, const std::string& key) {
        auto item = TreeViewItem(name, key);
        addItem(item);
    }

    void TreeViewGroup::_setupProperties() {
        this->key = "";
        this->backgroundColor = Color::transparent;

        d_expandGroupButton = MakeRef<Button>();
        d_expandGroupButton->cornerRadius = 0;
        d_expandGroupButton->horizontalAlignment = HAFill;
        d_expandGroupButton->borderColor = Color::transparent;
        d_expandGroupButton->backgroundColor = Color::transparent;
        d_expandGroupButton->hoverOnColor = Color(120, 120, 120, 80);
        d_expandGroupButton->mousePressedColor = Color(160, 160, 160, 80);
        d_expandGroupButton->label->alignment = "left";
        d_expandGroupButton->label->verticalPadding = 4;
        d_expandGroupButton->on("clicked", &TreeViewGroup::_expandButtonOnClick, this);
        name.on("propertyChanged", &TreeViewGroup::_onGroupNameChanged, this);
        addChild(d_expandGroupButton);

        name = "group";
        name.forwardEmittedEvents(this);

        cursorType = CursorType::Hand;

        on("itemSelected", &TreeViewGroup::_onItemSelected, this);
    }

    void TreeViewGroup::_onItemSelected(Shared<Event> e) {
        auto selectedKey = e->get<std::string>("key");

        // Remove highlights from unselected item buttons
        for (auto& [item, btn] : d_items) {
            if (selectedKey != item.key) {
                _removeHighlightFromItemButton(btn.get());
            }
        }

        if (selectedKey != key) {
            _removeHighlightFromItemButton(d_expandGroupButton.get());
        }
    }

    void TreeViewGroup::_onGroupNameChanged(Shared<Event> e) {
        std::string prefix = rightArrowUtf8Prefix;
        if (d_groupOpened) {
            prefix = downArrowUtf8Prefix;
        }

        d_expandGroupButton->label->text = prefix + name.get();
    }

    void TreeViewGroup::_expandButtonOnClick(Shared<Event> e) {
        if (d_groupOpened) {
            _removeAllHighlights();

            auto& children = _getChildren();
            for (uint64_t i = 1; i < children.size(); ++i) {
                children.at(i)->hide();
            }

            d_expandGroupButton->label->text = rightArrowUtf8Prefix + name.get();
            d_groupOpened = false;
        } else {
            auto& children = _getChildren();
            for (uint64_t i = 1; i < children.size(); ++i) {
                children.at(i)->show();
            }

            d_expandGroupButton->label->text = downArrowUtf8Prefix + name.get();
            d_groupOpened = true;
        }

        // Highlight the group button
        _highlightItemButton(d_expandGroupButton.get());

        fireEvent("itemSelected", {
            { "item", name },
            { "key", key}
        }, this);
    }

    void TreeViewGroup::_onItemClicked(Shared<Event> e) {
        // Dehighlight all items first
        _removeAllHighlights();

        // Highlight the current selected item
        auto button = static_cast<Button*>(e->target);
        _highlightItemButton(button);

        // Find the key for the current item
        std::string key = "";
        for (auto& [item, itemBtn] : d_items) {
            if (button->getID() == itemBtn->getID()) {
                key = item.key;
                break;
            }
        }

        fireEvent("itemSelected", {
            { "item", button->label->text.get() },
            { "key", key}
        }, this);
    }

    void TreeViewGroup::_onItemLostFocus(Shared<Event> e) {
        if (persistentSelection) {
            return;
        }

        auto button = static_cast<Button*>(e->target);
        _removeHighlightFromItemButton(button);
    }

    void TreeViewGroup::_removeAllHighlights() {
        // Remove highlights from nested sub-groups
        for (auto& [item, subGroup] : d_subGroups) {
            subGroup->_removeAllHighlights();
        }

        // Remove highlights from item buttons
        for (auto& [item, btn] : d_items) {
            _removeHighlightFromItemButton(btn.get());
        }

        // Remove hightlight from the group expansiom button
        _removeHighlightFromItemButton(d_expandGroupButton.get());
    }

    void TreeViewGroup::_highlightItemButton(Button* btn) {
        btn->backgroundColor = Color(160, 160, 160, 80);
        btn->borderColor = Color::white;
    }

    void TreeViewGroup::_removeHighlightFromItemButton(Button* btn) {
        btn->backgroundColor = Color::transparent;
        btn->borderColor = Color::transparent;
    }

    bool TreeViewGroup::removeNodeByKey(const std::string& key) {
        // Check if the name is from the item
        for (auto& it : d_items) {
            auto& treeItem = it.first;
            if (treeItem.key == key) {
                d_items.erase(it.first);
                return true;
            }
        }

        // Check if the name is from the subgroup
        for (auto& it : d_subGroups) {
            auto& treeItem = it.first;
            auto& subGroup = it.second;

            if (treeItem.key == key) {
                d_items.erase(it.first);
                return true;
            } else {
                // Check if an inner group has a child with the given key
                if (subGroup->removeNodeByKey(key)) {
                    return true;
                }
            }
        }

        return false;
    }

    Shared<TreeViewGroup> TreeViewGroup::findGroupByKey(const std::string& key) {
        for (auto& it : d_subGroups) {
            auto& treeItem = it.first;
            auto& subGroup = it.second;

            if (treeItem.key == key) {
                return subGroup;
            } else {
                // Check if an inner group node has a child with the given key
                auto innerGroup = subGroup->findGroupByKey(key);
                if (innerGroup) {
                    return innerGroup;
                }
            }
        }

        return nullptr;
    }

    TreeViewItem TreeViewGroup::findItemByKey(const std::string& key) {
        for (auto& it : d_items) {
            auto& treeItem = it.first;
            if (treeItem.key == key) {
                return treeItem;
            }
        }

        return TreeViewItem();
    }
} // namespace mc
