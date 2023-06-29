#include "TreeView.h"
#include "visuals/RectVisual.h"

namespace mc {
    TreeView::TreeView() {
        appendAllowedEvent("itemSelected");

        _createVisuals();
        _setupProperties();
    }

    Size TreeView::_measureSize() {
        Size contentSize = Size(0, 0);

        for (auto& child : _getChildren()) {
            auto childSizeWithMargins = child->getDesiredSizeWithMargins();

            // Keeping track of content size
            if (contentSize.width < childSizeWithMargins.width) {
                contentSize.width = childSizeWithMargins.width;
            }

            contentSize.height += childSizeWithMargins.height;
        }

        return contentSize;
    }

    void TreeView::_onArrangeChildren() {
        int32_t availablePosY = 0;

        for (auto& group : d_groups) {
            group->position = { 0, availablePosY };

            auto groupPanelSize = group->getDesiredSize();
            groupPanelSize.width = getComputedSize().width;

            group->setComputedSize(groupPanelSize);

            availablePosY += groupPanelSize.height;
        }
    }

    void TreeView::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void TreeView::_setupProperties() {
        backgroundColor = Color::gray; // default value
        backgroundColor.forwardEmittedEvents(this);

        itemTextColor = Color(); // default value
        itemTextColor.forwardEmittedEvents(this);

        itemBackgroundColor = Color(); // default value
        itemBackgroundColor.forwardEmittedEvents(this);

        itemSize = uint32_t(); // default value
        itemSize.forwardEmittedEvents(this);

        cursorType = CursorType::Hand;
    }

    void TreeView::addGroup(Shared<TreeViewGroup> group) {
        d_groups.push_back(group);
        group->forwardEmittedEvent(this, "itemSelected");

        _addChild(group);
    }

    void TreeView::removeGroup(Shared<TreeViewGroup> group) {
        removeNodeByKey(group->key);
    }

    bool TreeView::removeNodeByKey(const std::string& key) {
        for (auto it = d_groups.begin(); it != d_groups.end(); ++it) {
            auto group = *it;

            if (group->key == key) {
                d_groups.erase(it);
                _removeChild(group);
                break;
            } else {
                // Check if any of the inner nodes have the given key
                if (group->removeNodeByKey(key)) {
                    return true;
                }
            }
        }

        return false;
    }

    Shared<TreeViewGroup> TreeView::findGroupByKey(const std::string& key) {
        for (auto it = d_groups.begin(); it != d_groups.end(); ++it) {
            auto group = *it;

            if (group->key == key) {
                return group;
            } else {
                // Check if any of the inner groups have the given key
                auto innerGroup = group->findGroupByKey(key);
                if (innerGroup) {
                    return innerGroup;
                }
            }
        }

        return nullptr;
    }

    TreeViewItem TreeView::findItemByKey(const std::string& key) {
        for (auto it = d_groups.begin(); it != d_groups.end(); ++it) {
            auto group = *it;

            auto item = group->findItemByKey(key);
            if (!item.name.empty()) {
                return item;
            }
        }

        return TreeViewItem();
    }
} // namespace mc
