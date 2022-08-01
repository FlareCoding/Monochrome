#include "ScrollPanel.h"
#include <events/MouseEvents.h>

namespace mc {
    ScrollPanel::ScrollPanel() : Panel() {
        _createScrollbars();
        _setupScrollPanelProperties();
        _setupEventHandlers();
    }

    void ScrollPanel::_setupScrollPanelProperties() {
        verticalScroll = true;
        verticalScroll.forwardEmittedEvents(this);

        horizontalScroll = true;
        horizontalScroll.forwardEmittedEvents(this);

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
            d_verticalScrollbar->backgroundColor->a = 255;

            d_verticalScrollbarPressed = true;
            d_positionInWindow = getPositionInWindow();

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);

            int32_t mousePosY = mouseMovedEvent->getLocation().y;
            int32_t localMousePosY = mousePosY - d_positionInWindow.y;

            d_mousePositionWhenPressed.y = localMousePosY;
            d_verticalScrollbarPositionWhenPressed = d_verticalScrollbar->position;
        });

        d_verticalScrollbar->on("hoveredOn", [this](Shared<Event> e) {
            d_verticalScrollbar->backgroundColor->a = 255;
        });

        d_verticalScrollbar->on("mouseUp", [this](Shared<Event> e) {
            d_verticalScrollbar->backgroundColor->a = 160;
            d_verticalScrollbarPressed = false;
        });

        d_verticalScrollbar->on("mouseMoved", [this](Shared<Event> e) {
            // Only react to mouse "dragging"
            if (!d_verticalScrollbarPressed) {
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
            _adjustVerticalContentPosition();

            // Update the widget state and cause a re-render
            fireEvent("propertyChanged", Event::empty);

            d_mousePositionWhenPressed.y = localMousePosY;
            d_verticalScrollbarPositionWhenPressed.y = d_verticalScrollbar->position->y;
        });

        d_horizontalScrollbar->on("mouseDown", [this](Shared<Event> e) {
            d_horizontalScrollbar->backgroundColor->a = 255;

            d_horizontalScrollbarPressed = true;
            d_positionInWindow = getPositionInWindow();

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);

            int32_t mousePosX = mouseMovedEvent->getLocation().x;
            int32_t localMousePosX = mousePosX - d_positionInWindow.x;

            d_mousePositionWhenPressed.x = localMousePosX;
            d_horizontalScrollbarPositionWhenPressed = d_horizontalScrollbar->position;
        });

        d_horizontalScrollbar->on("hoveredOn", [this](Shared<Event> e) {
            d_horizontalScrollbar->backgroundColor->a = 255;
        });

        d_horizontalScrollbar->on("mouseUp", [this](Shared<Event> e) {
            d_horizontalScrollbar->backgroundColor->a = 160;
            d_horizontalScrollbarPressed = false;
        });

        d_horizontalScrollbar->on("mouseMoved", [this](Shared<Event> e) {
            // Only react to mouse "dragging"
            if (!d_horizontalScrollbarPressed) {
                return;
            }

            auto mouseMovedEvent = std::static_pointer_cast<MouseMovedEvent>(e);
            int32_t mousePosX = mouseMovedEvent->getLocation().x;
            int32_t localMousePosX = mousePosX - d_positionInWindow.x;

            int32_t diffX = localMousePosX - d_mousePositionWhenPressed.x;
            d_horizontalScrollbar->position->x = d_horizontalScrollbarPositionWhenPressed.x + diffX;

            // Sanity check the scrollbar position
            _verifyScrollbars();

            // Adjust the content panel position
            _adjustHorizontalContentPosition();

            // Update the widget state and cause a re-render
            fireEvent("propertyChanged", Event::empty);

            d_mousePositionWhenPressed.x = localMousePosX;
            d_horizontalScrollbarPositionWhenPressed.x = d_horizontalScrollbar->position->x;
        });

        on("hoveredOff", [this](Shared<Event> e) {
            d_verticalScrollbar->backgroundColor->a = 160;
            d_verticalScrollbarPressed = false;

            d_horizontalScrollbar->backgroundColor->a = 160;
            d_horizontalScrollbarPressed = false;
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
        _verifyVerticalScrollbar();
        _verifyHorizontalScrollbar();
    }

    void ScrollPanel::_verifyVerticalScrollbar() {
        if (!verticalScroll.get()) {
            d_verticalScrollbar->size->height = 0;
            return;
        }

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

    void ScrollPanel::_verifyHorizontalScrollbar() {
        if (!horizontalScroll.get()) {
            d_horizontalScrollbar->size->width = 0;
            return;
        }

        // Calculate horizontal scrollbar position
        d_horizontalScrollbar->position->y = size->height - d_scrollbarThickness;

        // Calculate vertical scrollbar height
        float widthPercentageOfContent =
            static_cast<float>(size->width) / static_cast<float>(content->size->width);

        uint32_t horizontalScrollbarWidth = 0;
        if (widthPercentageOfContent < 1.0f) {
            horizontalScrollbarWidth =
                static_cast<uint32_t>(widthPercentageOfContent * static_cast<float>(size->width));
        }

        d_horizontalScrollbar->size->width = horizontalScrollbarWidth;

        // Sanity checking vertical scrollbar position
        if (d_horizontalScrollbar->position->x < 0) {
            d_horizontalScrollbar->position->x = 0;
        }

        if (d_horizontalScrollbar->position->x >
            static_cast<int32_t>(size->width - horizontalScrollbarWidth)
        ) {
            d_horizontalScrollbar->position->x = size->width - horizontalScrollbarWidth;
        }
    }

    void ScrollPanel::_createScrollbars() {
        d_verticalScrollbar = MakeRef<Button>();
        d_verticalScrollbar->text = "";
        d_verticalScrollbar->size = { d_scrollbarThickness, 0 };
        d_verticalScrollbar->zIndex = 65000;
        d_verticalScrollbar->backgroundColor = Color::darkGray;
        d_verticalScrollbar->backgroundColor->a = 160;
        addChild(d_verticalScrollbar);

        d_horizontalScrollbar = MakeRef<Button>();
        d_horizontalScrollbar->text = "";
        d_horizontalScrollbar->size = { 0, d_scrollbarThickness };
        d_horizontalScrollbar->zIndex = 65000;
        d_horizontalScrollbar->backgroundColor = Color::darkGray;
        d_horizontalScrollbar->backgroundColor->a = 160;
        addChild(d_horizontalScrollbar);
    }

    void ScrollPanel::_adjustVerticalContentPosition() {
        // Get total distance that a scrollbar can move
        auto scrollbarMovableArea = size->height - d_verticalScrollbar->size->height;

        // Determine how much the scrollbar is scrolled
        float percentageScrolled =
            static_cast<float>(d_verticalScrollbar->position->y) /
            static_cast<float>(scrollbarMovableArea);

        // Get total scrollable distance of the content panel
        auto contentHeightDiff = content->size->height - size->height;

        // Calculate how much the content panel should be scrolled
        auto scrollAmount = static_cast<float>(contentHeightDiff) * percentageScrolled;

        // Adjust the content panel's position
        content->position->y = -static_cast<int32_t>(scrollAmount);
    }

    void ScrollPanel::_adjustHorizontalContentPosition() {
        // Get total distance that a scrollbar can move
        auto scrollbarMovableArea = size->width - d_horizontalScrollbar->size->width;

        // Determine how much the scrollbar is scrolled
        float percentageScrolled =
            static_cast<float>(d_horizontalScrollbar->position->x) /
            static_cast<float>(scrollbarMovableArea);

        // Get total scrollable distance of the content panel
        auto contentWidthDiff = content->size->width - size->width;

        // Calculate how much the content panel should be scrolled
        auto scrollAmount = static_cast<float>(contentWidthDiff) * percentageScrolled;

        // Adjust the content panel's position
        content->position->x = -static_cast<int32_t>(scrollAmount);
    }

} // namespace mc
