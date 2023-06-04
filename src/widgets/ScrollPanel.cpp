#include "ScrollPanel.h"
#include <events/MouseEvents.h>

namespace mc {
    ScrollPanel::ScrollPanel() {
        _setupProperties();
    }

    Size ScrollPanel::_measureSize() {
        Size contentSize = Size(0, 0);
        auto& children = _getChildren();
        if (children.size() > d_privateWidgets) {
            auto& content = children.at(0);
            contentSize = content->getDesiredSizeWithMargins();

            auto visibleSize = Size(fixedWidth, fixedHeight);

            if (visibleSize.width == NOT_SET) {
                visibleSize.width = std::min(contentSize.width, maxWidth.get());
            }

            if (visibleSize.height == NOT_SET) {
                visibleSize.height = std::min(contentSize.height, maxHeight.get());
            }

            // Determine if there should be space for a vertical scrollbar
            if (visibleSize.height != NOT_SET) {
                if (contentSize.height > visibleSize.height) {
                    contentSize.width += d_scrollbarTrackSize;
                    _showVerticalScrollElements();

                    // Since it was previously invisible, it's measure step was skipped
                    d_verticalScrollbar->markLayoutDirty();
                    d_verticalScrollbar->measure();
                } else {
                    _hideVerticalScrollElements();
                }
            }

            // Determine if there should be space for a horizontal scrollbar
            if (visibleSize.width != NOT_SET) {
                auto widthOffset = d_verticalScrollbar->visible ? d_scrollbarTrackSize : 0;

                if (contentSize.width - widthOffset > visibleSize.width) {
                    contentSize.height += d_scrollbarTrackSize;
                    _showHorizontalScrollElements();

                    // Since it was previously invisible, it's measure step was skipped
                    d_horizontalScrollbar->markLayoutDirty();
                    d_horizontalScrollbar->measure();
                } else {
                    _hideHorizontalScrollElements();
                }
            }
        }

        return contentSize;
    }

    void ScrollPanel::_onArrangeChildren() {
        auto computedSize = getComputedSize();

        auto& children = _getChildren();
        auto& content = children.at(0);

        // Finalize the content bounds
        content->position->x = content->marginLeft;

        content->setComputedSize(content->getDesiredSize());

        // Finalize the vertical scrollbar track
        d_verticalScrollbarTrack->position = Position(
            computedSize.width - d_scrollbarTrackSize,
            0
        );

        d_verticalScrollbarTrack->setComputedSize(
            Size(d_scrollbarTrackSize, computedSize.height)
        );

        // Finalize the position of the vertical scroll UP button
        d_verticalTrackUpButton->position = Position(
            computedSize.width - d_scrollbarTrackSize,
            0
        );

        // Finalize the position of the vertical scroll DOWN button
        d_verticalTrackDownButton->position = Position(
            computedSize.width - d_scrollbarTrackSize,
            computedSize.height - d_scrollbarTrackSize
        );

        // Finalize the position of the vertical scrollbar
        d_verticalScrollbar->position->x =
            computedSize.width - d_scrollbarTrackSize + d_verticalScrollbar->marginLeft;

        // Finalize the size of the vertical scrollbar
        auto verticalScrollbarSize = d_verticalScrollbar->getDesiredSize();
        verticalScrollbarSize.height = _calculateVerticalScrollbarSize();
        d_verticalScrollbar->setComputedSize(verticalScrollbarSize);

        // Finalize the horizontal scrollbar track
        d_horizontalScrollbarTrack->position = Position(
            0,
            computedSize.height - d_scrollbarTrackSize
        );

        d_horizontalScrollbarTrack->setComputedSize(
            Size(computedSize.width, d_scrollbarTrackSize)
        );

        // Finalize the position of the horizontal scroll LEFT button
        d_horizontalTrackLeftButton->position = Position(
            0,
            computedSize.height - d_scrollbarTrackSize
        );

        // Finalize the position of the horizontal scroll RIGHT button
        d_horizontalTrackRightButton->position = Position(
            computedSize.width - d_scrollbarTrackSize,
            computedSize.height - d_scrollbarTrackSize
        );

        // Check if the vertical track scroll DOWN button is present,
        // and if so, shift the horizontal track button to the left.
        if (d_verticalTrackDownButton->visible) {
            d_horizontalTrackRightButton->position->x -= d_scrollbarTrackSize;
        }

        // Finalize the position of the horizontal scrollbar
        d_horizontalScrollbar->position->y =
            computedSize.height - d_scrollbarTrackSize + d_verticalScrollbar->marginTop;

        // Finalize the size of the horizontal scrollbar
        auto horizontalScrollbarSize = d_horizontalScrollbar->getDesiredSize();
        horizontalScrollbarSize.width = _calculateHorizontalScrollbarSize();
        d_horizontalScrollbar->setComputedSize(horizontalScrollbarSize);

        // If autoscroll is enabled, scroll the content all the way down
        if (autoscroll.get() && content->getComputedSize().height > computedSize.height) {
            // Calculate how much scrolling distance is required
            int32_t scrollAmount = content->getComputedSize().height - computedSize.height;
            scrollAmount += content->position->y - position->y;
            scrollAmount *= -1;

            scrollContentVertically(scrollAmount);
        }
    }

    void ScrollPanel::_setupProperties() {
        // Required for scrollbars to be draggable
        markMouseDraggable();

        // Vertical track background panel
        d_verticalScrollbarTrack = MakeRef<StackPanel>();
        d_verticalScrollbarTrack->zIndex = std::numeric_limits<uint32_t>::max() - 1;
        d_verticalScrollbarTrack->fixedWidth = d_scrollbarTrackSize;
        scrollbarTracksColor.forwardAssignment(&d_verticalScrollbarTrack->backgroundColor);
        _addChild(d_verticalScrollbarTrack);

        // Vertical track UP button
        d_verticalTrackUpButton = MakeRef<Button>();
        d_verticalTrackUpButton->zIndex = std::numeric_limits<uint32_t>::max();
        d_verticalTrackUpButton->label->text = "▲";
        d_verticalTrackUpButton->cornerRadius = 0;
        d_verticalTrackUpButton->borderColor = Color::transparent;
        d_verticalTrackUpButton->setComputedSize(
            Size(d_scrollbarTrackSize, d_scrollbarTrackSize)
        );
        d_verticalTrackUpButton->on("clicked", [this](Shared<Event> e) {
            auto scrollAmountF = static_cast<float>(getComputedSize().height) * 0.08f;
            auto scrollAmount = static_cast<int32_t>(scrollAmountF);

            scrollContentVertically(scrollAmount);
        });
        _addChild(d_verticalTrackUpButton);

        // Vertical track DOWN button
        d_verticalTrackDownButton = MakeRef<Button>();
        d_verticalTrackDownButton->zIndex = std::numeric_limits<uint32_t>::max();
        d_verticalTrackDownButton->label->text = "▼";
        d_verticalTrackDownButton->cornerRadius = 0;
        d_verticalTrackDownButton->borderColor = Color::transparent;
        d_verticalTrackDownButton->setComputedSize(
            Size(d_scrollbarTrackSize, d_scrollbarTrackSize)
        );
        d_verticalTrackDownButton->on("clicked", [this](Shared<Event> e) {
            auto scrollAmountF = static_cast<float>(getComputedSize().height) * 0.08f;
            auto scrollAmount = static_cast<int32_t>(scrollAmountF) * -1;

            scrollContentVertically(scrollAmount);
        });
        _addChild(d_verticalTrackDownButton);

        // Vertical scrollbar
        d_verticalScrollbar = MakeRef<Button>();
        d_verticalScrollbar->zIndex = std::numeric_limits<uint32_t>::max();
        d_verticalScrollbar->fixedWidth = d_scrollbarTrackSize - 4;
        d_verticalScrollbar->position->y = d_scrollbarTrackSize + 2;
        d_verticalScrollbar->label->text = "";
        d_verticalScrollbar->cornerRadius = 0;
        d_verticalScrollbar->marginLeft = 2;
        d_verticalScrollbar->marginRight = 2;
        d_verticalScrollbar->marginTop = 2;
        d_verticalScrollbar->marginBottom = 2;
        d_verticalScrollbar->borderColor = Color::transparent;
        d_verticalScrollbar->on("mouseDown", &ScrollPanel::_verticalScrollbarOnMouseDown, this);
        d_verticalScrollbar->on("mouseUp", &ScrollPanel::_verticalScrollbarOnMouseUp, this);
        d_verticalScrollbar->on("mouseMoved", &ScrollPanel::_verticalScrollbarOnMouseMoved, this);
        d_verticalScrollbar->markMouseDraggable();
        _addChild(d_verticalScrollbar);

        // Initially hide the vertical scroll elements
        _hideVerticalScrollElements();

        // Horizontal track background panel
        d_horizontalScrollbarTrack = MakeRef<StackPanel>();
        d_horizontalScrollbarTrack->zIndex = std::numeric_limits<uint32_t>::max() - 1;
        d_horizontalScrollbarTrack->fixedHeight = d_scrollbarTrackSize;
        scrollbarTracksColor.forwardAssignment(&d_horizontalScrollbarTrack->backgroundColor);
        _addChild(d_horizontalScrollbarTrack);

        // Horizontal track LEFT button
        d_horizontalTrackLeftButton = MakeRef<Button>();
        d_horizontalTrackLeftButton->zIndex = std::numeric_limits<uint32_t>::max();
        d_horizontalTrackLeftButton->label->text = "◀";
        d_horizontalTrackLeftButton->cornerRadius = 0;
        d_horizontalTrackLeftButton->borderColor = Color::transparent;
        d_horizontalTrackLeftButton->setComputedSize(
            Size(d_scrollbarTrackSize, d_scrollbarTrackSize)
        );
        d_horizontalTrackLeftButton->on("clicked", [this](Shared<Event> e) {
            auto scrollAmountF = static_cast<float>(getComputedSize().width) * 0.08f;
            auto scrollAmount = static_cast<int32_t>(scrollAmountF);

            scrollContentHorizontally(scrollAmount);
        });
        _addChild(d_horizontalTrackLeftButton);

        // Horizontal track RIGHT button
        d_horizontalTrackRightButton = MakeRef<Button>();
        d_horizontalTrackRightButton->zIndex = std::numeric_limits<uint32_t>::max();
        d_horizontalTrackRightButton->label->text = "►";
        d_horizontalTrackRightButton->cornerRadius = 0;
        d_horizontalTrackRightButton->borderColor = Color::transparent;
        d_horizontalTrackRightButton->setComputedSize(
            Size(d_scrollbarTrackSize, d_scrollbarTrackSize)
        );
        d_horizontalTrackRightButton->on("clicked", [this](Shared<Event> e) {
            auto scrollAmountF = static_cast<float>(getComputedSize().width) * 0.08f;
            auto scrollAmount = static_cast<int32_t>(scrollAmountF) * -1;

            scrollContentHorizontally(scrollAmount);
        });
        _addChild(d_horizontalTrackRightButton);

        // Horizontal scrollbar
        d_horizontalScrollbar = MakeRef<Button>();
        d_horizontalScrollbar->zIndex = std::numeric_limits<uint32_t>::max();
        d_horizontalScrollbar->fixedHeight = d_scrollbarTrackSize - 4;
        d_horizontalScrollbar->position->x = d_scrollbarTrackSize + 2;
        d_horizontalScrollbar->label->text = "";
        d_horizontalScrollbar->cornerRadius = 0;
        d_horizontalScrollbar->marginLeft = 2;
        d_horizontalScrollbar->marginRight = 2;
        d_horizontalScrollbar->marginTop = 2;
        d_horizontalScrollbar->marginBottom = 2;
        d_horizontalScrollbar->borderColor = Color::transparent;
        d_horizontalScrollbar->on("mouseDown", &ScrollPanel::_horizontalScrollbarOnMouseDown, this);
        d_horizontalScrollbar->on("mouseUp", &ScrollPanel::_horizontalScrollbarOnMouseUp, this);
        d_horizontalScrollbar->on(
            "mouseMoved", &ScrollPanel::_horizontalScrollbarOnMouseMoved, this
        );
        d_horizontalScrollbar->markMouseDraggable();
        _addChild(d_horizontalScrollbar);

        // Initially hide the horizontal scroll elements
        _hideHorizontalScrollElements();

        // Setting up ScrollPanel public properties
        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        scrollbarTracksColor = Color::lightGray;
        scrollbarTracksColor.forwardEmittedEvents(this);

        scrollbarColor.forwardAssignment(&d_verticalScrollbar->backgroundColor);
        scrollbarColor.forwardAssignment(&d_horizontalScrollbar->backgroundColor);
        scrollbarColor = Color::gray;

        autoscroll = false;
        autoscroll.forwardEmittedEvents(this);

        scrollbarTrackButtonBackground.forwardAssignment(
            &d_verticalTrackDownButton->backgroundColor);

        scrollbarTrackButtonBackground.forwardAssignment(
            &d_verticalTrackUpButton->backgroundColor);

        scrollbarTrackButtonBackground.forwardAssignment(
            &d_horizontalTrackLeftButton->backgroundColor);

        scrollbarTrackButtonBackground.forwardAssignment(
            &d_horizontalTrackRightButton->backgroundColor);

        scrollbarTrackButtonBackground = Color::gray;

        scrollbarTrackButtonColor.forwardAssignment(&d_verticalTrackDownButton->label->color);
        scrollbarTrackButtonColor.forwardAssignment(&d_verticalTrackUpButton->label->color);
        scrollbarTrackButtonColor.forwardAssignment(&d_horizontalTrackLeftButton->label->color);
        scrollbarTrackButtonColor.forwardAssignment(&d_horizontalTrackRightButton->label->color);
        scrollbarTrackButtonColor = Color::white;

        on("mouseScrolled", [this](Shared<Event> e) {
            // Get the direction of the scroll event
            auto scrollDelta = e->get<int32_t>("deltaY") * d_scrollWheelSensitivity;

            // Scroll the content
            scrollContentVertically(scrollDelta);
        });
    }

    void ScrollPanel::_clampContentPosition() {
        auto content = getChild(0);
        auto contentSize = content->getComputedSize();

        auto visibleSize = getComputedSize();
        visibleSize.width -= d_scrollbarTrackSize;

        auto maxVerticalScrollAmount =
            static_cast<int32_t>(_getMaxPossibleVerticalScroll());

        auto maxHorizontalScrollAmount =
            static_cast<int32_t>(_getMaxPossibleHorizontalScroll());

        // Vertical sanitation
        if (contentSize.height > visibleSize.height) {
            if (content->position->y > 0) {
                content->position->y = 0;
            }

            if (content->position->y < -maxVerticalScrollAmount) {
                content->position->y = -maxVerticalScrollAmount;
            }
        }

        // Horizontal sanitation
        if (contentSize.width > visibleSize.width) {
            if (content->position->x > 0) {
                content->position->x = 0;
            }

            if (content->position->x < -maxHorizontalScrollAmount) {
                content->position->x = -maxHorizontalScrollAmount;
            }
        }
    }

    uint32_t ScrollPanel::_calculateVerticalScrollbarSize() {
        auto content = getChild(0);
        auto visibleSize = getComputedSize();

        auto contentHeightF = static_cast<float>(content->getComputedSize().height);
        auto visibleHeightF = static_cast<float>(
            visibleSize.height -
            d_verticalScrollbar->marginTop - d_verticalScrollbar->marginBottom -
            (d_scrollbarTrackSize * 2)
        );

        auto percentage = visibleHeightF / contentHeightF;
        auto scrollbarHeight = visibleHeightF * percentage;

        return static_cast<uint32_t>(scrollbarHeight);
    }

    void ScrollPanel::_calculateVerticalScrollbarPosition() {
        auto scrollbarMovingArea = _getVerticalScrollbarMovableDistance();
        auto scrolledPercentage = _getVerticalScrollbarScrolledPercentage();

        auto scrollbarPos = scrollbarMovingArea * scrolledPercentage;

        d_verticalScrollbar->position->y =
            d_scrollbarTrackSize + d_verticalScrollbar->marginTop +
            static_cast<int32_t>(scrollbarPos);
    }

    uint32_t ScrollPanel::_calculateHorizontalScrollbarSize() {
        auto content = getChild(0);
        auto visibleSize = getComputedSize();

        auto contentWidthF = static_cast<float>(content->getComputedSize().width);
        auto visibleWidthF = static_cast<float>(
            d_horizontalTrackRightButton->position->x -
            d_horizontalScrollbar->marginRight - d_horizontalScrollbar->marginLeft -
            d_scrollbarTrackSize
        );

        auto percentage = visibleWidthF / contentWidthF;
        auto scrollbarWidth = visibleWidthF * percentage;

        return static_cast<uint32_t>(scrollbarWidth);
    }

    void ScrollPanel::_calculateHorizontalScrollbarPosition() {
        auto scrollbarMovingArea = _getHorizontalScrollbarMovableDistance();
        auto scrolledPercentage = _getHorizontalScrollbarScrolledPercentage();

        auto scrollbarPos = scrollbarMovingArea * scrolledPercentage;

        d_horizontalScrollbar->position->x =
            d_scrollbarTrackSize + d_horizontalScrollbar->marginLeft +
            static_cast<int32_t>(scrollbarPos);
    }

    void ScrollPanel::_verticalScrollbarOnMouseDown(Shared<Event> e) {
        auto mbe = std::static_pointer_cast<MouseButtonEvent>(e);

        d_positionInWindow = getPositionInWindow();
        d_mousePressLocation = mbe->getLocation() - d_positionInWindow;

        auto content = getChild(0);
        d_preScrollContentPosition = content->position->y;

        d_verticalScrollbarPressed = true;
    }

    void ScrollPanel::_verticalScrollbarOnMouseUp(Shared<Event> e) {
        d_verticalScrollbarPressed = false;
    }

    void ScrollPanel::_verticalScrollbarOnMouseMoved(Shared<Event> e) {
        if (!d_verticalScrollbarPressed) {
            return;
        }

        auto mme = std::static_pointer_cast<MouseMovedEvent>(e);

        auto content = getChild(0);
        auto contentSize = content->getComputedSize();

        // Get local mouse position
        auto mousePos = mme->getLocation() - d_positionInWindow;

        // Calculate vertical moved distance
        auto movedDistance = mousePos.y - d_mousePressLocation.y;

        // Calculate scroll percentage from moved distance
        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea = _getVerticalScrollbarMovableDistance();
        auto maxVerticalScrollAmount = _getMaxPossibleVerticalScroll();

        float movedPercentage =
            static_cast<float>(movedDistance) / static_cast<float>(scrollbarMovingArea);

        float contentOffsetF = movedPercentage * static_cast<float>(maxVerticalScrollAmount) * -1;
        auto contentOffset = static_cast<int32_t>(contentOffsetF);

        // Perform the scroll
        content->position->y = d_preScrollContentPosition + contentOffset;

        // Make sure the content isn't moved beyond its scrollable bounds
        _clampContentPosition();

        // Update the position of the vertical scrollbar
        _calculateVerticalScrollbarPosition();

        // Request a widget redraw
        fireEvent("propertyChanged", Event::empty);
    }

    void ScrollPanel::_horizontalScrollbarOnMouseDown(Shared<Event> e) {
        auto mbe = std::static_pointer_cast<MouseButtonEvent>(e);

        d_positionInWindow = getPositionInWindow();
        d_mousePressLocation = mbe->getLocation() - d_positionInWindow;

        auto content = getChild(0);
        d_preScrollContentPosition = content->position->x;

        d_horizontalScrollbarPressed = true;
    }

    void ScrollPanel::_horizontalScrollbarOnMouseUp(Shared<Event> e) {
        d_horizontalScrollbarPressed = false;
    }

    void ScrollPanel::_horizontalScrollbarOnMouseMoved(Shared<Event> e) {
        if (!d_horizontalScrollbarPressed) {
            return;
        }

        auto mme = std::static_pointer_cast<MouseMovedEvent>(e);

        auto content = getChild(0);
        auto contentSize = content->getComputedSize();

        // Get local mouse position
        auto mousePos = mme->getLocation() - d_positionInWindow;

        // Calculate vertical moved distance
        auto movedDistance = mousePos.x - d_mousePressLocation.x;

        // Calculate scroll percentage from moved distance
        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea = _getHorizontalScrollbarMovableDistance();
        auto maxHorizontalScrollAmount = _getMaxPossibleHorizontalScroll();

        float movedPercentage =
            static_cast<float>(movedDistance) / static_cast<float>(scrollbarMovingArea);

        float contentOffsetF =
            movedPercentage * static_cast<float>(maxHorizontalScrollAmount) * -1;

        auto contentOffset = static_cast<int32_t>(contentOffsetF);

        // Perform the scroll
        content->position->x = d_preScrollContentPosition + contentOffset;

        // Make sure the content isn't moved beyond its scrollable bounds
        _clampContentPosition();

        // Update the position of the vertical scrollbar
        _calculateHorizontalScrollbarPosition();

        // Request a widget redraw
        fireEvent("propertyChanged", Event::empty);
    }

    void ScrollPanel::_showVerticalScrollElements() {
        d_verticalTrackUpButton->visible = true;
        d_verticalTrackDownButton->visible = true;
        d_verticalScrollbar->visible = true;
        d_verticalScrollbarTrack->visible = true;
    }

    void ScrollPanel::_hideVerticalScrollElements() {
        d_verticalTrackUpButton->visible = false;
        d_verticalTrackDownButton->visible = false;
        d_verticalScrollbar->visible = false;
        d_verticalScrollbarTrack->visible = false;
    }

    void ScrollPanel::_showHorizontalScrollElements() {
        d_horizontalTrackLeftButton->visible = true;
        d_horizontalTrackRightButton->visible = true;
        d_horizontalScrollbar->visible = true;
        d_horizontalScrollbarTrack->visible = true;
    }

    void ScrollPanel::_hideHorizontalScrollElements() {
        d_horizontalTrackLeftButton->visible = false;
        d_horizontalTrackRightButton->visible = false;
        d_horizontalScrollbar->visible = false;
        d_horizontalScrollbarTrack->visible = false;
    }

    float ScrollPanel::_getVerticalScrollbarScrolledPercentage() {
        auto content = getChild(0);

        auto maxVerticalScrollAmount = _getMaxPossibleVerticalScroll();
        auto scrolledAmount = -1 * content->position->y;

        auto scrolledPercentage = static_cast<float>(scrolledAmount) /
            static_cast<float>(maxVerticalScrollAmount);

        return scrolledPercentage;
    }

    uint32_t ScrollPanel::_getVerticalScrollbarMovableDistance() {
        auto content = getChild(0);
        auto contentSize = content->getComputedSize();

        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea = visibleSize.height -
            (d_scrollbarTrackSize * 2) -
            d_verticalScrollbar->getComputedSizeWithMargins().height;

        return scrollbarMovingArea;
    }

    uint32_t ScrollPanel::_getMaxPossibleVerticalScroll() {
        auto content = getChild(0);
        auto contentSize = content->getComputedSize();
        auto visibleSize = getComputedSize();

        return contentSize.height - visibleSize.height;
    }

    float ScrollPanel::_getHorizontalScrollbarScrolledPercentage() {
        auto content = getChild(0);

        auto maxHorizontalScrollAmount = _getMaxPossibleHorizontalScroll();
        auto scrolledAmount = -1 * content->position->x;

        auto scrolledPercentage = static_cast<float>(scrolledAmount) /
            static_cast<float>(maxHorizontalScrollAmount);

        return scrolledPercentage;
    }

    uint32_t ScrollPanel::_getHorizontalScrollbarMovableDistance() {
        auto content = getChild(0);
        auto contentSize = content->getComputedSize();

        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea =
            d_horizontalTrackRightButton->position->x -
            d_scrollbarTrackSize -
            d_horizontalScrollbar->getComputedSizeWithMargins().width;

        return scrollbarMovingArea;
    }

    uint32_t ScrollPanel::_getMaxPossibleHorizontalScroll() {
        auto content = getChild(0);
        auto contentSize = content->getComputedSize();
        auto visibleSize = getComputedSize();

        return contentSize.width - visibleSize.width;
    }

    void ScrollPanel::scrollContentVertically(int32_t amount) {
        CORE_ASSERT(getChildren().size() > d_privateWidgets, "ScrollPanel content not set");

        auto content = getChild(0);
        content->position->y += amount;

        // Make sure the content isn't moved beyond its scrollable bounds
        _clampContentPosition();

        // Update the position of the vertical scrollbar
        _calculateVerticalScrollbarPosition();
    }

    void ScrollPanel::scrollContentHorizontally(int32_t amount) {
        CORE_ASSERT(getChildren().size() > d_privateWidgets, "ScrollPanel content not set");

        auto content = getChild(0);
        content->position->x += amount;

        // Make sure the content isn't moved beyond its scrollable bounds
        _clampContentPosition();

        // Update the position of the horizontal scrollbar
        _calculateHorizontalScrollbarPosition();
    }
} // namespace mc
