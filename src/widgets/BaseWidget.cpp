#include "BaseWidget.h"

namespace mc {
    BaseWidget::BaseWidget() {
        d_uuid = utils::generateUUID();

        appendAllowedEvent("propertyChanged");
        appendAllowedEvent("zIndexChanged");
        appendAllowedEvent("layoutChanged");
        appendAllowedEvent("dynamicallyResized");
        appendAllowedEvent("mouseDown");
        appendAllowedEvent("mouseUp");
        appendAllowedEvent("mouseMoved");
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

        focused = true;
        focused.forwardEmittedEvents(this);

        cursorType = CursorType::Arrow;
        cursorType.forwardEmittedEvents(this);

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
        // At this point, the computed size
        // will be set for 'this' widget.
        _onArrangeChildren();

        // After arranging the children is done, all
        // direct children will have computed sizes, so
        // it's safe to arrange their internal children next.
        for (auto& child : _getChildren()) {
            child->arrangeChildren();
        }
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

    void BaseWidget::addCoreVisualElement(Shared<VisualElement> visual) {
        d_coreVisualElements.push_back(visual);
    }

    void BaseWidget::addOverlayVisualElement(Shared<VisualElement> visual) {
        d_overlayVisualElements.push_back(visual);
    }

    void BaseContainerWidget::addChild(Shared<BaseWidget> child) {
        _addChild(child);
    }

    bool BaseContainerWidget::removeChild(Shared<BaseWidget> child) {
        return _removeChild(child);
    }

    bool BaseContainerWidget::removeChild(uuid_t uuid) {
        return _removeChild(uuid);
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
} // namespace mc
