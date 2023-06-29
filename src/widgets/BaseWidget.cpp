#include "BaseWidget.h"
#include <core/InternalFlags.h>

namespace mc {
    // Special widget-id dictionary that maps user-defined IDs to widget instances
    static std::map<std::string, Shared<BaseWidget>> s_widgetUserIdRegistry;

    BaseWidget::BaseWidget() {
        d_uuid = utils::generateUUID();

        appendAllowedEvent("propertyChanged");
        appendAllowedEvent("zIndexChanged");
        appendAllowedEvent("layoutChanged");
        appendAllowedEvent("dynamicallyResized");
        appendAllowedEvent("mouseDown");
        appendAllowedEvent("mouseUp");
        appendAllowedEvent("mouseMoved");
        appendAllowedEvent("mouseScrolled");
        appendAllowedEvent("clicked");
        appendAllowedEvent("hoveredOn");
        appendAllowedEvent("hoveredOff");
        appendAllowedEvent("keyDown");
        appendAllowedEvent("keyUp");
        appendAllowedEvent("gainedFocus");
        appendAllowedEvent("lostFocus");
        appendAllowedEvent("requestedFocusGain");
        appendAllowedEvent("requestedFocusLoss");

        _setDefaultProperties();
    }

    void BaseWidget::_setDefaultProperties() {
        zIndex = 0;
        zIndex.on("propertyChanged", [this](auto e) {
            this->fireEvent("zIndexChanged", Event::empty);
        });

        visible = true;
        visible.forwardEmittedEvents(this);

        visible.on("propertyChanged", [this](auto e) {
            this->markLayoutDirty();
        });

        focused = true;
        focused.forwardEmittedEvents(this);

        cursorType = CursorType::Arrow;
        cursorType.forwardEmittedEvents(this);

        dockAnchor = DockAnchor::Left;
        dockAnchor.forwardEmittedEvents(this);

        dockAnchor.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        position = { 0, 0 };
        position.forwardEmittedEvents(this);

        fixedWidth = NOT_SET;
        fixedWidth.forwardEmittedEvents(this);

        minWidth = 0;
        minWidth.forwardEmittedEvents(this);

        maxWidth = 65000;
        maxWidth.forwardEmittedEvents(this);

        fixedHeight = NOT_SET;
        fixedHeight.forwardEmittedEvents(this);

        minHeight = 0;
        minHeight.forwardEmittedEvents(this);

        maxHeight = 65000;
        maxHeight.forwardEmittedEvents(this);

        fixedWidth.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        minWidth.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        maxWidth.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        fixedHeight.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        minHeight.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        maxHeight.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        marginTop = 0;
        marginTop.forwardEmittedEvents(this);

        marginBottom = 0;
        marginBottom.forwardEmittedEvents(this);

        marginLeft = 0;
        marginLeft.forwardEmittedEvents(this);

        marginRight = 0;
        marginRight.forwardEmittedEvents(this);

        horizontalAlignment = HorizontalAlignment::HALeft;
        horizontalAlignment.forwardEmittedEvents(this);

        verticalAlignment = VerticalAlignment::VATop;
        verticalAlignment.forwardEmittedEvents(this);

        horizontalAlignment.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        verticalAlignment.on("propertyChanged", [this](auto e) {
            this->fireEvent("layoutChanged", Event::empty);
        });

        on("layoutChanged", [this](auto e) {
            d_isLayoutDirty = true;
        });
    }

    Size BaseWidget::getDesiredSize() {
        return d_desiredSize;
    }

    Size BaseWidget::getDesiredSizeWithMargins() {
        return {
            marginLeft + d_desiredSize.width + marginRight,
            marginTop + d_desiredSize.height + marginBottom
        };
    }

    Size BaseWidget::getComputedSize() {
        return d_computedSize;
    }

    Size BaseWidget::getComputedSizeWithMargins() {
        return {
            marginLeft + d_computedSize.width + marginRight,
            marginTop + d_computedSize.height + marginBottom
        };
    }

    void BaseWidget::measure() {
        // Ignore measuring children if the layout
        // has not been marked as dirty or been changed.
        if (!isLayoutDirty()) {
            return;
        }

        // Check the special case when the widget is
        // invisible and therefore doesn't take up any space.
        if (!visible.get()) {
            d_desiredSize = Size(0, 0);
            return;
        }

        for (auto& child : _getChildren()) {
            child->measure();
        }

        d_desiredSize = _measureSize();

        // Check against a fixed specified width
        if (fixedWidth != NOT_SET) {
            d_desiredSize.width = fixedWidth;
        }

        // Check against a fixed specified height
        if (fixedHeight != NOT_SET) {
            d_desiredSize.height = fixedHeight;
        }

        // Min/max width constraints
        if (d_desiredSize.width > maxWidth) {
            d_desiredSize.width = maxWidth;

        } else if (d_desiredSize.width < minWidth) {
            d_desiredSize.width = minWidth;
        }

        // Min/max height constraints
        if (d_desiredSize.height > maxHeight) {
            d_desiredSize.height = maxHeight;

        } else if (d_desiredSize.height < minHeight) {
            d_desiredSize.height = minHeight;
        }
    }

    void BaseWidget::arrangeChildren() {
        // Ignore arranging children if the layout
        // has not been marked as dirty or been changed.
        if (!isLayoutDirty() && !isContainer()) {
            return;
        }

        // At this point, the computed size
        // will be set for 'this' widget.
        _onArrangeChildren();

        // After arranging the children is done, all
        // direct children will have computed sizes, so
        // it's safe to arrange their internal children next.
        for (auto& child : _getChildren()) {
            child->arrangeChildren();
        }

        d_isLayoutDirty = false;
    }

    void BaseWidget::setComputedSize(const Size& size) {
        d_computedSize = size;
        _onSetComputedSize(size);
    }

    Position BaseWidget::getPositionInWindow() const {
        Position result = this->position.get();

        BaseWidget* parent = d_parent;
        while (parent) {
            result.x += parent->position->x;
            result.y += parent->position->y;

            parent = parent->d_parent;
        }

        return result;
    }

    void BaseWidget::focus() {
        fireEvent("requestedFocusGain", {
            { "target", this }
        });
    }

    void BaseWidget::unfocus() {
        fireEvent("requestedFocusLoss", Event::empty);
    }

    void BaseWidget::show() {
        visible = true;
        markLayoutDirty();
    }

    void BaseWidget::hide() {
        visible = false;
        markLayoutDirty();
    }

    void BaseWidget::markLayoutDirty() {
        d_isLayoutDirty = true;
    }

    void BaseWidget::markMouseDraggable() {
        setInternalFlag(d_internalFlags, InternalWidgetFlag::IsMouseDraggable, true);
    }

    void BaseWidget::addCoreVisualElement(Shared<VisualElement> visual) {
        d_coreVisualElements.push_back(visual);
    }

    void BaseWidget::addOverlayVisualElement(Shared<VisualElement> visual) {
        d_overlayVisualElements.push_back(visual);
    }

    void BaseContainerWidget::addChild(Shared<BaseWidget> child) {
        _addChild(child);
    }

    void BaseContainerWidget::addChildOffline(Shared<BaseWidget> child) {
        _addChildOffline(child);
    }

    bool BaseContainerWidget::removeChild(Shared<BaseWidget> child) {
        return _removeChild(child);
    }

    bool BaseContainerWidget::removeChild(uuid_t uuid) {
        return _removeChild(uuid);
    }

    bool BaseContainerWidget::removeChildOffline(Shared<BaseWidget> child) {
        return _removeChildOffline(child);
    }

    bool BaseContainerWidget::removeChildOffline(uuid_t uuid) {
        return _removeChildOffline(uuid);
    }

    void BaseContainerWidget::removeAllChildren() {
        _removeAllChildren();
    }

    Shared<BaseWidget> BaseContainerWidget::findChild(uuid_t uuid) {
        return _findChild(uuid);
    }

    Shared<BaseWidget> BaseContainerWidget::getChild(uint64_t index) {
        return _getChild(index);
    }

    std::vector<Shared<BaseWidget>>& BaseContainerWidget::getChildren() {
        return _getChildren();
    }

    void BaseContainerWidget::signalLayoutChanged() {
        fireEvent("layoutChanged", Event::empty);
    }

    Shared<BaseWidget> BaseContainerWidget::deepSearchWidgetByUuid(uuid_t id) {
        for (auto& child : _getChildren()) {
            if (child->getID() == id) {
                return child;
            }

            // Check if the child is a container, and if so,
            // see if the child's subtree contains the target.
            if (child->isContainer()) {
                auto container = std::static_pointer_cast<BaseContainerWidget>(child);
                auto subtreeChildWidget = container->deepSearchWidgetByUuid(id);

                // If the subtreeChildWidget is a valid widget, return it
                if (subtreeChildWidget) {
                    return subtreeChildWidget;
                }
            }
        }

        return nullptr;
    }

    void registerWidgetWithUserId(const std::string& id, Shared<BaseWidget> widget) {
        s_widgetUserIdRegistry[id] = widget;
    }

    Shared<BaseWidget> __getBaseWidgetById(const std::string& id) {
        if (s_widgetUserIdRegistry.find(id) == s_widgetUserIdRegistry.end()) {
            return nullptr;
        }

        return s_widgetUserIdRegistry.at(id);
    }
} // namespace mc
