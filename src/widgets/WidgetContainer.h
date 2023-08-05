#pragma once
#include <events/EventEmitter.h>
#include <utils/uuid.h>
#include <algorithm>

namespace mc {
template <typename T>
class PrivateContainer : public EventEmitter {
friend class Renderer;

protected:
    PrivateContainer<T>() {
        appendAllowedEvent("childAdded");
        appendAllowedEvent("childRemoved");
        appendAllowedEvent("layoutChanged");
    }

    virtual ~PrivateContainer<T>() = default;

    // Adds a child to the list of widgets
    // @param child Child element to be added
    void _addChild(Shared<T> child) {
        _addChildOffline(child);

        fireEvent("childAdded", {
            { "child", child.get() }
        });

        fireEvent("layoutChanged", Event::empty);
    }

    // Adds a child to the list of widgets without causing
    // expensive events such as "layoutChanged" event firing.
    // @param child Child element to be added
    void _addChildOffline(Shared<T> child) {
        CORE_ASSERT((child.get() != this), "Cannot add widget as its own child");
        CORE_ASSERT(!child->getParent(), "Cannot add child, child widget already has a parent");
        CORE_ASSERT(
            !_findChild(child->getID()),
            "Cannot add widget, widget with the given UUID already exists"
        );

        d_children.push_back(child);
        _orderChildrenByZIndex();

        child->setParent(static_cast<T*>(this));
        child->forwardEmittedEvent(this, "propertyChanged");
        child->forwardEmittedEvent(this, "requestedFocusGain");
        child->forwardEmittedEvent(this, "requestedFocusLoss");
        child->forwardEmittedEvent(this, "layoutChanged");

        // If the child's z-index changes, the container
        // should reorder all children in ascending order.
        child->on("zIndexChanged", [this](auto e) {
            _orderChildrenByZIndex();
        });
    }

    // Adds a child to the list of widgets at a given index
    // @param child Child element to be added
    // @param idx Index at which to insert the child
    void _insertChild(Shared<T> child, size_t idx) {
        _insertChildOffline(child, idx);

        fireEvent("childAdded", {
            { "child", child.get() }
        });

        fireEvent("layoutChanged", Event::empty);
    }

    // Inserts a child to the list of widgets at a given index without
    // causing expensive events such as "layoutChanged" event firing.
    // @param child Child element to be added
    // @param idx Index at which to insert the child
    void _insertChildOffline(Shared<T> child, size_t idx) {
        CORE_ASSERT((child.get() != this), "Cannot add widget as its own child");
        CORE_ASSERT(!child->getParent(), "Cannot add child, child widget already has a parent");
        CORE_ASSERT(
            !_findChild(child->getID()),
            "Cannot add widget, widget with the given UUID already exists"
        );

        d_children.insert(d_children.begin() + idx, child);
        _orderChildrenByZIndex();

        child->setParent(static_cast<T*>(this));
        child->forwardEmittedEvent(this, "propertyChanged");
        child->forwardEmittedEvent(this, "requestedFocusGain");
        child->forwardEmittedEvent(this, "requestedFocusLoss");
        child->forwardEmittedEvent(this, "layoutChanged");

        // If the child's z-index changes, the container
        // should reorder all children in ascending order.
        child->on("zIndexChanged", [this](auto e) {
            _orderChildrenByZIndex();
        });
    }

    // Removes a child from the list of children
    // @param child Child element to be removed
    // @returns Status of whether a child has been removed successfully
    bool _removeChild(Shared<T> child) {
        return _removeChild(child->getID());
    }

    // Removes a child from the list of children
    // @param uuid ID of the child to remove
    // @returns Status of whether a child has been removed successfully
    bool _removeChild(uuid_t uuid) {
        for (auto it = d_children.begin(); it != d_children.end(); ++it) {
            if (it->get()->getID() == uuid) {
                T* child = it->get();

                // Reset the child's parent
                child->setParent(nullptr);

                // Remove the zIndexChanged event listenr
                child->off("zIndexChanged");

                // Erase the child from the list
                d_children.erase(it);

                // Fire the childRemoved event
                fireEvent("childRemoved", {
                    { "child", child }
                });

                fireEvent("layoutChanged", Event::empty);
                return true;
            }
        }

        return false;
    }

    // Removes a child from the list of children without causing
    // expensive events such as "layoutChanged" event firing.
    // @param child Child element to be removed
    // @returns Status of whether a child has been removed successfully
    bool _removeChildOffline(Shared<T> child) {
        return _removeChildOffline(child->getID());
    }

    // Removes a child from the list of children without causing
    // expensive events such as "layoutChanged" event firing.
    // @param uuid ID of the child to remove
    // @returns Status of whether a child has been removed successfully
    bool _removeChildOffline(uuid_t uuid) {
        for (auto it = d_children.begin(); it != d_children.end(); ++it) {
            if (it->get()->getID() == uuid) {
                T* child = it->get();

                // Reset the child's parent
                child->setParent(nullptr);

                // Remove the zIndexChanged event listenr
                child->off("zIndexChanged");

                // Erase the child from the list
                d_children.erase(it);

                return true;
            }
        }

        return false;
    }

    // Removes all children
    void _removeAllChildren() {
        while (d_children.size()) {
            auto firstChild = _getChild(0);
            _removeChildOffline(firstChild->getID());
        }

        _signalLayoutChanged();
    }

    // Attemts to find a child given its ID
    // @param uuid ID of the child to find
    // @returns Shared pointer to the child if it was found, nullptr otherwise
    Shared<T> _findChild(uuid_t uuid) {
        for (auto& child : d_children) {
            if (child->getID() == uuid) {
                return child;
            }
        }

        return nullptr;
    }

    // Attemts to find a child given its index in the list of children
    // @param uuid Index in the list of children of the child widget
    // @returns Shared pointer to the child if it was found, nullptr otherwise
    Shared<T> _getChild(uint64_t index) {
        return d_children.at(index);
    }

    // @returns A list of all direct children widgets
    inline std::vector<Shared<T>>& _getChildren() { return d_children; }

    // Fires a layout changed event
    inline void _signalLayoutChanged() {
        fireEvent("layoutChanged", Event::empty);
    }

private:
    std::vector<Shared<T>> d_children;

    inline void _orderChildrenByZIndex() {
        std::sort(d_children.begin(), d_children.end(),
            [](Shared<T> a, Shared<T> b) {
                return a->zIndex.get() < b->zIndex.get();
        });
    }
};

class BaseWidget;
using PrivateWidgetContainer = PrivateContainer<BaseWidget>;
} // namespace mc
