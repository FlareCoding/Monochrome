#include "ScrollPanel.h"
#include <events/MouseEvents.h>

namespace mc {
    ScrollPanel::ScrollPanel() : Panel() {
        _createScrollbars();
        _setupScrollPanelProperties();
        _setupEventHandlers();
    }

    void ScrollPanel::_setupScrollPanelProperties() {
        contentWidth = 0;
        contentWidth.forwardEmittedEvents(this);

        contentHeight = 0;
        contentHeight.forwardEmittedEvents(this);
    }

    void ScrollPanel::_setupEventHandlers() {
        size.on("propertyChanged", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
        });

        contentWidth.on("propertyChanged", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
        });

        contentHeight.on("propertyChanged", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
        });

        on("dynamicallyResized", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
        });

        on("childAdded", [this](Shared<Event> e) {
            auto child = e->get<BaseWidget*>("child");
            child->position.on("propertyChanged", [this, child](Shared<Event> e) {
                child->position->x = d_contentOrigin.x + child->position->x;
                child->position->y = d_contentOrigin.y + child->position->y;
            });

            child->position->x = d_contentOrigin.x + child->position->x;
            child->position->y = d_contentOrigin.y + child->position->y;
        });

        on("childRemoved", [this](Shared<Event> e) {
            auto child = e->get<BaseWidget*>("child");
            child->position.off("propertyChanged");
        });

        d_verticalScrollbar->on("mouseDown", [this](Shared<Event> e) {
            d_mousePressed = true;
            d_positionInWindow = getPositionInWindow();

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);

            int32_t mousePosY = mouseMovedEvent->getLocation().y;
            int32_t localMousePosY = mousePosY - d_positionInWindow.y;

            d_mousePositionWhenPressed.y = localMousePosY;
            d_verticalScrollbarPositionWhenPressed = d_verticalScrollbar->position;
        });

        d_verticalScrollbar->on("mouseUp", [this](Shared<Event> e) {
            d_mousePressed = false;
        });

        d_verticalScrollbar->on("mouseMoved", [this](Shared<Event> e) {
            // Only react to mouse "dragging"
            if (!d_mousePressed) {
                return;
            }

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);
            int32_t mousePosY = mouseMovedEvent->getLocation().y;
            int32_t localMousePosY = mousePosY - d_positionInWindow.y;

            int32_t diffY = localMousePosY - d_mousePositionWhenPressed.y;
            d_verticalScrollbar->position->y = d_verticalScrollbarPositionWhenPressed.y + diffY;
            d_contentOrigin.y = d_contentOrigin.y - diffY;
            printf("%i\n", d_contentOrigin.y);

            _verifyScrollbars();
            fireEvent("propertyChanged", Event::empty);

            d_mousePositionWhenPressed.y = localMousePosY;
            d_verticalScrollbarPositionWhenPressed.y = d_verticalScrollbar->position->y;
        });
    }

    void ScrollPanel::_verifyContentAreaBounds() {
        if (contentWidth < size->width) {
            contentWidth = size->width;
        }

        if (contentHeight < size->height) {
            contentHeight = size->height;
        }

        if (d_contentOrigin.x > 0) { d_contentOrigin.x = 0; }
        if (d_contentOrigin.y > 0) { d_contentOrigin.y = 0; }

        _verifyScrollbars();
    }

    void ScrollPanel::_verifyScrollbars() {
        // Calculate vertical scrollbar position
        d_verticalScrollbar->position->x = size->width - d_scrollbarThickness;

        // Calculate vertical scrollbar height
        float heightPercentageOfContent =
            static_cast<float>(size->height) / static_cast<float>(contentHeight.get());

        uint32_t verticalScrollbarHeight = 0;
        if (heightPercentageOfContent < 1.0f) {
            verticalScrollbarHeight =
                static_cast<uint32_t>(heightPercentageOfContent * static_cast<float>(size->height));
        }

        d_verticalScrollbar->size->height = verticalScrollbarHeight;

        // Sanity checking vertical scrollbar position
        if (d_verticalScrollbar->position->y < 0) {
            d_verticalScrollbar->position->y = 0;
        }

        if (d_verticalScrollbar->position->y >
            static_cast<int32_t>(size->height - verticalScrollbarHeight)
        ) {
            d_verticalScrollbar->position->y = size->height - verticalScrollbarHeight;
        }
    }

    void ScrollPanel::_createScrollbars() {
        d_verticalScrollbar = MakeRef<Button>();
        d_verticalScrollbar->text = "";
        d_verticalScrollbar->size = { d_scrollbarThickness, 0 };
        d_verticalScrollbar->zIndex = 65000;
        d_verticalScrollbar->backgroundColor = Color::darkGray;
        addChild(d_verticalScrollbar);
    }

} // namespace mc
