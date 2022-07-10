#include "BaseContainerWidget.h"

namespace mc
{
    BaseContainerWidget::BaseContainerWidget() {
        appendAllowedEvent("childAdded");
        appendAllowedEvent("childRemoved");
    }

    void BaseContainerWidget::addChild(Shared<BaseWidget> child) {
        CORE_ASSERT(!child->getParent(), "Cannot add child, child widget already has a parent");
        CORE_ASSERT(!findChild(child->getID()), "Cannot add widget, widget with the given UUID already exists");

        d_children.push_back(child);
        child->setParent(this);
        child->forwardEmittedEvent(this, "propertyChanged");
        child->forwardEmittedEvent(this, "requestedFocusGain");
        child->forwardEmittedEvent(this, "requestedFocusLoss");

        fireEvent("childAdded", Event::empty);
    }

    void BaseContainerWidget::insertChild(Shared<BaseWidget> child, uint64_t index) {
        CORE_ASSERT(!child->getParent(), "Cannot add child, child widget already has a parent");
        CORE_ASSERT(!findChild(child->getID()), "Cannot add widget, widget with the given UUID already exists");

        d_children.insert(d_children.begin() + index, child);
        child->setParent(this);
        child->forwardEmittedEvents(this);

        fireEvent("childAdded", Event::empty);
    }

    bool BaseContainerWidget::removeChild(Shared<BaseWidget> child) {
        return removeChild(child->getID());
    }

    bool BaseContainerWidget::removeChild(uuid_t uuid) {
        for (auto it = d_children.begin(); it != d_children.end(); ++it) {
            if (it->get()->getID() == uuid) {
                d_children.erase(it);

                it->get()->setParent(nullptr);
                fireEvent("childRemoved", Event::empty);
                return true;
            }
        }

        return false;
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
    
}
