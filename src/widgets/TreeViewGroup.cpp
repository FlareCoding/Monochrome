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

        d_subGroups.insert({ subGroup->name.get(), subGroup });
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
        itemButton->label->text = item;
        itemButton->marginLeft = TREE_INDENT_MARGIN;
        itemButton->zIndex = 1;
        itemButton->on("clicked", &TreeViewGroup::_onItemClicked, this);
        itemButton->on("lostFocus", &TreeViewGroup::_onItemLostFocus, this);
        addChild(itemButton);

        d_items.insert({ item, itemButton });
    }

    void TreeViewGroup::_setupProperties() {
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
    }

    void TreeViewGroup::_onGroupNameChanged(Shared<Event> e) {
        std::string prefix = " ► ";
        if (d_groupOpened) {
            prefix = " ▼ ";
        }

        d_expandGroupButton->label->text = prefix + name.get();
    }

    void TreeViewGroup::_expandButtonOnClick(Shared<Event> e) {
        if (d_groupOpened) {
            auto& children = _getChildren();
            for (uint64_t i = 1; i < children.size(); ++i) {
                children.at(i)->visible = false;
            }

            d_expandGroupButton->label->text = " ► " + name.get();
            d_groupOpened = false;
        } else {
            auto& children = _getChildren();
            for (uint64_t i = 1; i < children.size(); ++i) {
                children.at(i)->visible = true;
            }

            d_expandGroupButton->label->text = " ▼ " + name.get();
            d_groupOpened = true;
        }
    }

    void TreeViewGroup::_onItemClicked(Shared<Event> e) {
        auto button = static_cast<Button*>(e->target);

        button->backgroundColor = Color(160, 160, 160, 80);
        button->borderColor = Color::white;

        fireEvent("itemSelected", {
            { "item", button->label->text.get() }
        }, this);
    }

    void TreeViewGroup::_onItemLostFocus(Shared<Event> e) {
        auto button = static_cast<Button*>(e->target);

        button->backgroundColor = Color::transparent;
        button->borderColor = Color::transparent;
    }

    bool TreeViewGroup::remove(const std::string& name) {
        // Check if the name is from the item
        if (d_items.find(name) != d_items.end()) {
            auto itemButton = d_items.at(name);
            removeChild(itemButton);

            d_items.erase(name);
            return true;
        }

        // Check if the name is from the subgroup
        if (d_subGroups.find(name) != d_subGroups.end()) {
            auto subGroup = d_subGroups.at(name);
            removeChild(subGroup);

            d_subGroups.erase(name);
            return true;
        }

        return false;
    }

    Shared<TreeViewGroup> TreeViewGroup::getGroup(const std::string& name) {
        if (d_subGroups.find(name) == d_subGroups.end()) {
            return nullptr;
        }

        return d_subGroups.at(name);
    }
} // namespace mc
