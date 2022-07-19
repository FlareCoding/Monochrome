#include "BaseContainerWidget.h"
#include <algorithm>

namespace mc {
    BaseContainerWidget::BaseContainerWidget() {
        appendAllowedEvent("childAdded");
        appendAllowedEvent("childRemoved");
    }

    void BaseContainerWidget::addChild(Shared<BaseWidget> child) {
        CORE_ASSERT((child.get() != this), "Cannot add widget as its own child");
        CORE_ASSERT(!child->getParent(), "Cannot add child, child widget already has a parent");
        CORE_ASSERT(
            !findChild(child->getID()),
            "Cannot add widget, widget with the given UUID already exists"
        );

        d_children.push_back(child);
        _orderChildrenByZIndex();

        child->setParent(this);
        child->forwardEmittedEvent(this, "propertyChanged");
        child->forwardEmittedEvent(this, "requestedFocusGain");
        child->forwardEmittedEvent(this, "requestedFocusLoss");

        // If the child's z-index changes, the container
        // should reorder all children in ascending order.
        child->on("zIndexChanged", [this](auto e) {
            _orderChildrenByZIndex();
        });

        if (child->isContainer()) {
            child->forwardEmittedEvent(this, "childAdded");
            child->forwardEmittedEvent(this, "childRemoved");
        }

        fireEvent("childAdded", {
            { "child", child.get() }
        });
    }

    bool BaseContainerWidget::removeChild(Shared<BaseWidget> child) {
        return removeChild(child->getID());
    }

    bool BaseContainerWidget::removeChild(uuid_t uuid) {
        for (auto it = d_children.begin(); it != d_children.end(); ++it) {
            if (it->get()->getID() == uuid) {
                BaseWidget* widget = it->get();

                // Reset the child's parent
                widget->setParent(nullptr);

                // Remove the zIndexChanged event listenr
                widget->off("zIndexChanged");

                // Erase the child from the list
                d_children.erase(it);

                // Fire the childRemoved event
                fireEvent("childRemoved", {
                    { "child", widget }
                });
                return true;
            }
        }

        return false;
    }

    void BaseContainerWidget::removeAllChildren() {
        while (d_children.size()) {
            auto firstChild = getChild(0);
            removeChild(firstChild);
        }
    }

    Shared<BaseWidget> BaseContainerWidget::findChild(uuid_t uuid) {
        for (auto& widget : d_children) {
            if (widget->getID() == uuid) {
                return widget;
            }
        }

        return nullptr;
    }

    Shared<BaseWidget> BaseContainerWidget::getChild(uint64_t index) {
        return d_children.at(index);
    }

    void BaseContainerWidget::_orderChildrenByZIndex() {
        std::sort(d_children.begin(), d_children.end(),
        [](Shared<BaseWidget> a, Shared<BaseWidget> b) {
            return a->zIndex.get() < b->zIndex.get();
        });
    }
} // namespace mc
