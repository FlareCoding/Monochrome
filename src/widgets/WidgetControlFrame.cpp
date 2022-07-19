#include "WidgetControlFrame.h"
#include <rendering/RenderTarget.h>
#include <events/MouseEvents.h>

#define RESIZE_FRAME_ANCHOR_TOP_LEFT        1
#define RESIZE_FRAME_ANCHOR_TOP_MIDDLE      2
#define RESIZE_FRAME_ANCHOR_TOP_RIGHT       3
#define RESIZE_FRAME_ANCHOR_MIDDLE_RIGHT    4
#define RESIZE_FRAME_ANCHOR_BOTTOM_RIGHT    5
#define RESIZE_FRAME_ANCHOR_BOTTOM_MIDDLE   6
#define RESIZE_FRAME_ANCHOR_BOTTOM_LEFT     7
#define RESIZE_FRAME_ANCHOR_MIDDLE_LEFT     8

namespace mc {
    WidgetControlFrame::WidgetControlFrame() : CustomRenderedContainerWidget() {
        content = nullptr;
        content.forwardEmittedEvents(this);

        widgetDraggable = true;

        enabled = true;
        enabled.forwardEmittedEvents(this);

        _setupEventHandlers();
    }

    void WidgetControlFrame::_setupEventHandlers() {
        content.on("propertyChanged", [this](Shared<Event> e) {
            if (content.get()) {
                content.get()->off("widgetResized");
            }
            removeAllChildren();

            size = content->get()->size;
            content->get()->position = { d_anchorSquareSize, d_anchorSquareSize };

            addChild(content);
            content->get()->on("widgetResized", [this](Shared<Event> e) {
                size = content->get()->size;
            });
        });

        size.on("propertyChanged", [this](Shared<Event> e) {
            content->get()->size->width -= d_anchorSquareSize * 2;
            content->get()->size->height -= d_anchorSquareSize * 2;
        });

        on("mouseDown", [this](Shared<Event> e) {
            if (!content->get() || !enabled) {
                return;
            }

            auto mme = std::static_pointer_cast<MouseMovedEvent>(e);
            auto mouseLocation = mme->getLocation();
            Position localMouseLocation = {
                mouseLocation.x - position->x,
                mouseLocation.y - position->y
            };

            _onMouseDown(localMouseLocation);
        });

        on("mouseMoved", [this](Shared<Event> e) {
            if (!content->get() || !enabled) {
                return;
            }

            auto mme = std::static_pointer_cast<MouseMovedEvent>(e);
            auto mouseLocation = mme->getLocation();
            Position localMouseLocation = {
                mouseLocation.x - position->x,
                mouseLocation.y - position->y
            };

            _onMouseMoved(localMouseLocation);
        });

        on("mouseUp", [this](Shared<Event> e) {
            if (!content->get() || !enabled) {
                return;
            }

            _onMouseUp();
        });
    }

    void WidgetControlFrame::_onMouseDown(const Position& mousePos) {
        d_mouseAnchorPosition = mousePos;
        d_mousePressed = true;

        d_resizeAnchor = _getAnchorFromPosition(mousePos);
    }

    void WidgetControlFrame::_onMouseMoved(const Position& mousePos) {
        if (!d_mousePressed) {
            return;
        }

        cursorType = CursorType::Arrow;
        content->get()->cursorType = CursorType::Arrow;

        Position diff = {
            mousePos.x - d_mouseAnchorPosition.x,
            mousePos.y - d_mouseAnchorPosition.y
        };

        switch (d_resizeAnchor) {
        case RESIZE_FRAME_ANCHOR_TOP_LEFT: {
            // diff.x should be treated as negative
            if (static_cast<int32_t>(size->width) - diff.y > d_minAcceptableWidth) {
                position->x += diff.x;
                size->width -= diff.x;
                content->get()->size->width -= diff.x;
            }

            // diff.y should be treated as negative
            if (static_cast<int32_t>(size->height) - diff.y > d_minAcceptableHeight) {
                position->y += diff.y;
                size->height -= diff.y;
                content->get()->size->height -= diff.y;
            }

            break;
        }
        case RESIZE_FRAME_ANCHOR_BOTTOM_LEFT: {
            // diff.x should be treated as negative
            if (static_cast<int32_t>(size->width) - diff.y > d_minAcceptableWidth) {
                position->x += diff.x;
                size->width -= diff.x;
                content->get()->size->width -= diff.x;
            }

            // diff.y should be treated as positive
            if (static_cast<int32_t>(size->height) + diff.y > d_minAcceptableHeight) {
                size->height += diff.y;
                content->get()->size->height += diff.y;

                d_mouseAnchorPosition.y = mousePos.y;
            }

            break;
        }
        case RESIZE_FRAME_ANCHOR_TOP_RIGHT: {
            // diff.x should be treated as positive
            if (static_cast<int32_t>(size->width) + diff.y > d_minAcceptableWidth) {
                size->width += diff.x;
                content->get()->size->width += diff.x;

                d_mouseAnchorPosition.x = mousePos.x;
            }

            // diff.y should be treated as negative
            if (static_cast<int32_t>(size->height) - diff.y > d_minAcceptableHeight) {
                position->y += diff.y;
                size->height -= diff.y;
                content->get()->size->height -= diff.y;
            }

            break;
        }
        case RESIZE_FRAME_ANCHOR_BOTTOM_RIGHT: {
            // diff.x should be treated as positive
            if (static_cast<int32_t>(size->width) + diff.y > d_minAcceptableWidth) {
                size->width += diff.x;
                content->get()->size->width += diff.x;

                d_mouseAnchorPosition.x = mousePos.x;
            }

            // diff.y should be treated as positive
            if (static_cast<int32_t>(size->height) + diff.y > d_minAcceptableHeight) {
                size->height += diff.y;
                content->get()->size->height += diff.y;

                d_mouseAnchorPosition.y = mousePos.y;
            }

            break;
        }
        case RESIZE_FRAME_ANCHOR_TOP_MIDDLE: {
            // diff.y should be treated as negative
            if (static_cast<int32_t>(size->height) - diff.y < d_minAcceptableHeight) {
                break;
            }

            position->y += diff.y;
            size->height -= diff.y;
            content->get()->size->height -= diff.y;
            break;
        }
        case RESIZE_FRAME_ANCHOR_BOTTOM_MIDDLE: {
            // diff.y should be treated as positive
            if (static_cast<int32_t>(size->height) + diff.y < d_minAcceptableHeight) {
                break;
            }

            size->height += diff.y;
            content->get()->size->height += diff.y;

            d_mouseAnchorPosition = mousePos;
            break;
        }
        case RESIZE_FRAME_ANCHOR_MIDDLE_LEFT: {
            // diff.x should be treated as negative
            if (static_cast<int32_t>(size->width) - diff.y < d_minAcceptableWidth) {
                break;
            }

            position->x += diff.x;
            size->width -= diff.x;
            content->get()->size->width -= diff.x;
            break;
        }
        case RESIZE_FRAME_ANCHOR_MIDDLE_RIGHT: {
            // diff.x should be treated as positive
            if (static_cast<int32_t>(size->width) + diff.y < d_minAcceptableWidth) {
                break;
            }

            size->width += diff.x;
            content->get()->size->width += diff.x;

            d_mouseAnchorPosition = mousePos;
            break;
        }
        default: {
            if (widgetDraggable) {
                position->x += diff.x;
                position->y += diff.y;
            }
        }
        }
    }

    void WidgetControlFrame::_onMouseUp() {
        d_mousePressed = false;
    }

    int WidgetControlFrame::_getAnchorFromPosition(const Position& pos) {
        if (pos.x < d_anchorSquareSize * 2 && pos.y < d_anchorSquareSize * 2) {
            // Top left
            return RESIZE_FRAME_ANCHOR_TOP_LEFT;
        } else if (
            pos.x > static_cast<int32_t>(size->width / 2 - (d_anchorSquareSize * 2)) &&
            pos.x < static_cast<int32_t>(size->width / 2 + (d_anchorSquareSize * 2)) &&
            pos.y < d_anchorSquareSize * 2
        ) {
            // Top middle
            return RESIZE_FRAME_ANCHOR_TOP_MIDDLE;
        } else if (
            pos.x > static_cast<int32_t>(size->width - d_anchorSquareSize * 2) &&
            pos.y < d_anchorSquareSize * 2
        ) {
            // Top right
            return RESIZE_FRAME_ANCHOR_TOP_RIGHT;
        } else if (
            pos.x > static_cast<int32_t>(size->width - d_anchorSquareSize * 2) &&
            pos.y > static_cast<int32_t>(size->height / 2 - (d_anchorSquareSize * 2)) &&
            pos.y < static_cast<int32_t>(size->height / 2 + (d_anchorSquareSize * 2))
        ) {
            // Middle right
            return RESIZE_FRAME_ANCHOR_MIDDLE_RIGHT;
        } else if (
            pos.x > static_cast<int32_t>(size->width - d_anchorSquareSize * 2) &&
            pos.y > static_cast<int32_t>(size->height - (d_anchorSquareSize * 2))
        ) {
            // Bottom right
            return RESIZE_FRAME_ANCHOR_BOTTOM_RIGHT;
        } else if (
            pos.x > static_cast<int32_t>(size->width / 2 - (d_anchorSquareSize * 2)) &&
            pos.x < static_cast<int32_t>(size->width / 2 + (d_anchorSquareSize * 2)) &&
            pos.y > static_cast<int32_t>(size->height - (d_anchorSquareSize * 2))
        ) {
            // Bottom middle
            return RESIZE_FRAME_ANCHOR_BOTTOM_MIDDLE;
        } else if (
            pos.x < d_anchorSquareSize * 2 &&
            pos.y > static_cast<int32_t>(size->height - (d_anchorSquareSize * 2))
        ) {
            // Bottom left
            return RESIZE_FRAME_ANCHOR_BOTTOM_LEFT;
        } else if (
            pos.x < d_anchorSquareSize * 2 &&
            pos.y > static_cast<int32_t>(size->height / 2 - (d_anchorSquareSize * 2)) &&
            pos.y < static_cast<int32_t>(size->height / 2 + (d_anchorSquareSize * 2))
        ) {
            // Middle left
            return RESIZE_FRAME_ANCHOR_MIDDLE_LEFT;
        } else {
            return 0;
        }
    }

    void WidgetControlFrame::onRender(
        Shared<RenderTarget>& renderTarget,
        Position& parentPositionOffset,
        getWidgetBoundsFn_t getWidgetBounds
    ) {
        if (!content.get() || !enabled) {
            return;
        }

        auto [position, size] = getWidgetBounds(this, parentPositionOffset);

        // Draw the top and bottom resize anchors
        for (auto i = 0; i < 3; ++i) {
            auto xPos = position.x + (size.width / 2) * i;
            if (i == 2) {
                xPos -= d_anchorSquareSize;
            }

            renderTarget->drawRectangle(
                xPos, position.y,
                d_anchorSquareSize, d_anchorSquareSize,
                Color::yellow,
                0,
                true,
                0
            );

            renderTarget->drawRectangle(
                xPos,
                position.y + size.height - d_anchorSquareSize,
                d_anchorSquareSize, d_anchorSquareSize,
                Color::yellow,
                0,
                true,
                0
            );
        }

        // Draw the left middle resize anchor
        renderTarget->drawRectangle(
            position.x, position.y + size.height / 2 - (d_anchorSquareSize / 2),
            d_anchorSquareSize, d_anchorSquareSize,
            Color::yellow,
            0,
            true,
            0
        );

        // Draw the right middle resize anchor
        renderTarget->drawRectangle(
            position.x + size.width - d_anchorSquareSize,
            position.y + size.height / 2 - (d_anchorSquareSize / 2),
            d_anchorSquareSize, d_anchorSquareSize,
            Color::yellow,
            0,
            true,
            0
        );
    }
} // namespace mc
