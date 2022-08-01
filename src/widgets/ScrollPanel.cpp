#include "ScrollPanel.h"
#include <events/MouseEvents.h>

namespace mc {
    ScrollPanel::ScrollPanel() : Panel() {
        _createScrollbars();
        _setupScrollPanelProperties();
        _setupEventHandlers();
    }

    void ScrollPanel::_setupScrollPanelProperties() {
        content = MakeRef<Panel>();
        content->forwardEmittedEvents(this);
        content->position = { 0, 0 };
        addChild(content);

        backgroundColor.on("propertyChanged", [this](auto e) {
            content->backgroundColor = backgroundColor;
        });

        cornerRadius.on("propertyChanged", [this](auto e) {
            content->cornerRadius = cornerRadius;
        });
    }

    void ScrollPanel::_setupEventHandlers() {
        size.on("propertyChanged", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
        });

        on("dynamicallyResized", [this](Shared<Event> e) {
            _verifyContentAreaBounds();
            _verifyScrollbars();
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

            // Sanity check the scrollbar position
            _verifyScrollbars();

            // Adjust the content panel position
            auto scrollbarMovableArea = size->height - d_verticalScrollbar->size->height;
            float percentageScrolled =
                static_cast<float>(d_verticalScrollbar->position->y) /
                static_cast<float>(scrollbarMovableArea);

            auto contentHeightDiff = content->size->height - size->height;
            auto scrollAmount = static_cast<float>(contentHeightDiff) * percentageScrolled;

            content->position->y = -static_cast<int32_t>(scrollAmount);

            // Update the widget state and cause a re-render
            fireEvent("propertyChanged", Event::empty);

            d_mousePositionWhenPressed.y = localMousePosY;
            d_verticalScrollbarPositionWhenPressed.y = d_verticalScrollbar->position->y;
        });
    }

    void ScrollPanel::_verifyContentAreaBounds() {
        if (content->size->width < size->width) {
            content->size->width = size->width;
        }

        if (content->size->height < size->height) {
            content->size->height = size->height;
        }

        if (content->position->x > 0) { content->position->x = 0; }
        if (content->position->y > 0) { content->position->y = 0; }

        _verifyScrollbars();
    }

    void ScrollPanel::_verifyScrollbars() {
        // Calculate vertical scrollbar position
        d_verticalScrollbar->position->x = size->width - d_scrollbarThickness;

        // Calculate vertical scrollbar height
        float heightPercentageOfContent =
            static_cast<float>(size->height) / static_cast<float>(content->size->height);

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

        d_horizontalScrollbar = MakeRef<Button>();
        d_horizontalScrollbar->text = "";
        d_horizontalScrollbar->size = { 0, d_scrollbarThickness };
        d_horizontalScrollbar->zIndex = 65000;
        d_horizontalScrollbar->backgroundColor = Color::darkGray;
        addChild(d_horizontalScrollbar);
    }

} // namespace mc
