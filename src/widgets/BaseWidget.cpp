#include "BaseWidget.h"

namespace mc {
    BaseWidget::BaseWidget() {
        d_uuid = utils::generateUUID();

        appendAllowedEvent("propertyChanged");
        appendAllowedEvent("zIndexChanged");
        appendAllowedEvent("widgetResized");
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

        size = { 10, 10 };
        size.forwardEmittedEvents(this);
        size.on("propertyChanged", [this](auto e) {
            this->fireEvent("widgetResized", Event::empty);
        });

        marginTop = 0;
        marginTop.forwardEmittedEvents(this);

        marginBottom = 0;
        marginBottom.forwardEmittedEvents(this);

        marginLeft = 0;
        marginLeft.forwardEmittedEvents(this);

        marginRight = 0;
        marginRight.forwardEmittedEvents(this);
    }

    Size BaseWidget::getClientSize() {
        return {
            marginLeft + size->width + marginRight,
            marginTop + size->height + marginBottom
        };
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
