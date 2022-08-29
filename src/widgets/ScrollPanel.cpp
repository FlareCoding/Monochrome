#include "ScrollPanel.h"
#include <events/MouseEvents.h>

namespace mc {
    ScrollPanel::ScrollPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size ScrollPanel::_measureSize() {
        Size contentSize = Size(0, 0);
        auto& children = _getChildren();
        if (children.size() > d_privateWidgets) {
            auto& content = children.at(0);
            contentSize = content->getDesiredSizeWithMargins();

            auto visibleSize = Size(fixedWidth, fixedHeight);
            auto computedSize = getComputedSize();

            if (visibleSize.width == NOT_SET) {
                visibleSize.width = computedSize.width;
            }

            if (visibleSize.height == NOT_SET) {
                visibleSize.height = computedSize.height;
            }

            // Determine if there should be space for a vertical scrollbar
            if (visibleSize.height != NOT_SET) {
                if (contentSize.height > visibleSize.height) {
                    contentSize.width += d_scrollbarTrackSize;
                    _showVerticalScrollElements();
                }
                else {
                    _hideVerticalScrollElements();
                }
            }

            // Determine if there should be space for a horizontal scrollbar
            if (visibleSize.width != NOT_SET) {
                if (contentSize.width > maxWidth) {
                    contentSize.height += d_scrollbarTrackSize;
                } else {
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

        // Finalize the position and size of the vertical scrollbar
        d_verticalScrollbar->position->x =
            computedSize.width - d_scrollbarTrackSize + d_verticalScrollbar->marginLeft;

        auto verticalScrollbarSize = d_verticalScrollbar->getDesiredSize();
        verticalScrollbarSize.height = _calculateVerticalScrollbarSize();
        d_verticalScrollbar->setComputedSize(verticalScrollbarSize);
    }

    void ScrollPanel::_createVisuals() {
        // Setup the panel's body rectangle
        d_bodyVisual = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&d_bodyVisual->cornerRadius);
        backgroundColor.forwardAssignment(&d_bodyVisual->color);
        addCoreVisualElement(d_bodyVisual);
    }

    void ScrollPanel::_setupProperties() {
        // Required for scrollbars to be draggable
        markMouseDraggable();

        backgroundColor = Color::lightGray;
        backgroundColor.forwardEmittedEvents(this);

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

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
        d_verticalScrollbar->backgroundColor = Color::gray;
        d_verticalScrollbar->on("mouseDown", &ScrollPanel::_verticalScrollbarOnMouseDown, this);
        d_verticalScrollbar->on("mouseUp", &ScrollPanel::_verticalScrollbarOnMouseUp, this);
        d_verticalScrollbar->on("mouseMoved", &ScrollPanel::_verticalScrollbarOnMouseMoved, this);
        d_verticalScrollbar->markMouseDraggable();
        _addChild(d_verticalScrollbar);

        _hideVerticalScrollElements();
    }

    void ScrollPanel::_clampContentPosition() {
        auto& content = getChild(0);
        auto contentSize = content->getComputedSize();

        auto visibleSize = getComputedSize();
        visibleSize.width -= d_scrollbarTrackSize;

        auto maxVerticalScrollAmount = static_cast<int32_t>(contentSize.height - visibleSize.height);
        auto maxHorizontalScrollAmount = static_cast<int32_t>(contentSize.width - visibleSize.width);

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
        auto& content = getChild(0);
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
        auto& content = getChild(0);
        auto contentSize = content->getComputedSize();

        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea = visibleSize.height -
                                    (d_scrollbarTrackSize * 2) -
                                    d_verticalScrollbar->getComputedSize().height -
                                    d_verticalScrollbar->marginTop -
                                    d_verticalScrollbar->marginBottom;

        auto maxVerticalScrollAmount = contentSize.height - visibleSize.height;
        auto scrolledAmount = -1 * content->position->y;

        auto scrolledPercentage = static_cast<float>(scrolledAmount) /
                                static_cast<float>(maxVerticalScrollAmount);

        auto scrollbarPos = scrollbarMovingArea * scrolledPercentage;

        d_verticalScrollbar->position->y =
            d_scrollbarTrackSize + d_verticalScrollbar->marginTop +
            static_cast<int32_t>(scrollbarPos);
    }

    void ScrollPanel::_verticalScrollbarOnMouseDown(Shared<Event> e) {
        auto mbe = std::static_pointer_cast<MouseButtonEvent>(e);

        d_positionInWindow = getPositionInWindow();
        d_mousePressLocation = mbe->getLocation() - d_positionInWindow;
        
        auto& content = getChild(0);
        d_preScrollContentPosition = content->position->y;

        d_mousePressed = true;
    }

    void ScrollPanel::_verticalScrollbarOnMouseUp(Shared<Event> e) {
        d_mousePressed = false;
    }

    void ScrollPanel::_verticalScrollbarOnMouseMoved(Shared<Event> e) {
        if (!d_mousePressed) {
            return;
        }

        auto mme = std::static_pointer_cast<MouseMovedEvent>(e);

        auto& content = getChild(0);
        auto contentSize = content->getComputedSize();

        // Get local mouse position
        auto mousePos = mme->getLocation() - d_positionInWindow;
        
        // Calculate vertical moved distance
        auto movedDistance = mousePos.y - d_mousePressLocation.y;

        // Calculate scroll percentage from moved distance
        auto visibleSize = getComputedSize();

        auto scrollbarMovingArea = visibleSize.height -
            (d_scrollbarTrackSize * 2) -
            d_verticalScrollbar->getComputedSize().height -
            d_verticalScrollbar->marginTop -
            d_verticalScrollbar->marginBottom;

        auto maxVerticalScrollAmount = contentSize.height - visibleSize.height;

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

        fireEvent("propertyChanged", Event::empty);
    }

    void ScrollPanel::_showVerticalScrollElements() {
        d_verticalTrackUpButton->visible = true;
        d_verticalTrackDownButton->visible = true;
        d_verticalScrollbar->visible = true;
    }

    void ScrollPanel::_hideVerticalScrollElements() {
        d_verticalTrackUpButton->visible = false;
        d_verticalTrackDownButton->visible = false;
        d_verticalScrollbar->visible = false;
    }

    void ScrollPanel::scrollContentVertically(int32_t amount) {
        CORE_ASSERT(getChildren().size() > d_privateWidgets, "ScrollPanel content not set");

        auto& content = getChild(0);
        content->position->y += amount;

        // Make sure the content isn't moved beyond its scrollable bounds
        _clampContentPosition();

        // Update the position of the vertical scrollbar
        _calculateVerticalScrollbarPosition();
    }
} // namespace mc
