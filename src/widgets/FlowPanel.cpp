#include "FlowPanel.h"

namespace mc {
    FlowPanel::FlowPanel() : Panel() {
        _setupFlowpanelProperties();
        _setupEventHandlers();
    }

    void FlowPanel::_setupFlowpanelProperties() {
        layout = Horizontal;
        layout.forwardEmittedEvents(this);

        justifyContent = None;
        justifyContent.forwardEmittedEvents(this);

        wrapContents = true;
        wrapContents.forwardEmittedEvents(this);

        stretchContents = false;
        stretchContents.forwardEmittedEvents(this);

        autoSize = false;
        autoSize.forwardEmittedEvents(this);
    }

    void FlowPanel::_setupEventHandlers() {
        on("childAdded", [this](Shared<Event> e) {
            auto child = e->get<BaseWidget*>("child");
            child->size.on("propertyChanged", [this](Shared<Event> e) {
                _calculateChildrenDynamicPosition();
            });

            _calculateChildrenDynamicPosition();
        });

        on("childRemoved", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        size.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        layout.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        justifyContent.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        wrapContents.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        stretchContents.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });

        autoSize.on("propertyChanged", [this](Shared<Event> e) {
            _calculateChildrenDynamicPosition();
        });
    }

    void FlowPanel::_calculateChildrenDynamicPosition() {
        Frame availableInsertionFrame;
        _calculateNextChildPosition(availableInsertionFrame.position, nullptr);

        // Used to track maximum content size for a given
        // layout to be potentially used in the autoSize calculation.
        uint32_t maxContentSize = 0;

        for (auto& child : d_children) {
            // Update the child's position and size
            _calculateCurrentChildPosition(availableInsertionFrame.position, child.get());

            if (layout == Horizontal || layout == HorizontalReversed) {
                // Justify elements in a single row
                uint32_t rowElementCount = 0;
                uint32_t rowHeight = _calculateHorizontalRowHeight(
                    availableInsertionFrame.position.y,
                    rowElementCount
                );

                _justifyRowElementsHorizontally(
                    availableInsertionFrame.position.y,
                    rowElementCount
                );

                // If the contents need to be stretched, set each element's
                // height to be the same height as the flowpanel.
                if (stretchContents) {
                    child->size->height = size->height - child->marginBottom - child->marginTop;
                    child->fireEvent("dynamicallyResized", Event::empty);
                }

                // Keep track of the maximum content size
                auto totalChildHeight =
                    child->marginTop + child->size->height + child->marginBottom;

                if (child->position->y + totalChildHeight > maxContentSize) {
                    maxContentSize = child->position->y + totalChildHeight;
                }
            } else {
                // Logic gets simplified a lot since there
                // is no element wrapping in vertical layouts.
                _justifyElementsVertically();

                // If the contents need to be stretched, set each element's
                // width to be the same width as the flowpanel.
                if (stretchContents) {
                    child->size->width = size->width;
                    child->fireEvent("dynamicallyResized", Event::empty);
                }

                // Keep track of the maximum content size
                // *Note: in a vertical layout items are
                // always aligned with the left border.
                if (child->size->width > maxContentSize) {
                    maxContentSize = child->size->width;
                }
            }

            // If the autoSize property is true, then
            // resize the panel to be the size of the content.
            if (autoSize) {
                switch (layout) {
                    case Horizontal:
                    case HorizontalReversed: {
                        // Update the panel's height
                        size->height = maxContentSize;
                        break;
                    }
                    case Vertical:
                    case VerticalReversed: {
                        // Update the panel's width
                        size->width = maxContentSize;
                        break;
                    }
                    default: break;
                }

                // Indicate that the redraw is needed because
                // size's property observer wasn't affected.
                fireEvent("propertyChanged", Event::empty);
            }

            // Prepare the position anchor for the next child
            _calculateNextChildPosition(availableInsertionFrame.position, child.get());
        }
    }

    uint32_t FlowPanel::_calculateHorizontalRowHeight(
        int32_t targetRowPositionY,
        uint32_t& elementCount
    ) {
        uint32_t rowHeight = 0;

        for (auto& child : d_children) {
            // Ignore elements in a different row
            if (child->position->y - child->marginTop != targetRowPositionY) {
                continue;
            }

            // Track the element
            ++elementCount;

            // Track the greatest element height
            auto totalChildHeight = child->marginTop + child->size->height + child->marginBottom;

            if (totalChildHeight > rowHeight) {
                rowHeight = totalChildHeight;
            }
        }

        return rowHeight;
    }

    void FlowPanel::_calculateCurrentChildPosition(Position& pos, BaseWidget* child) {
        switch (layout) {
        case FlowLayout::Horizontal: {
            // Handle the overflow
            if (wrapContents) {
                if (pos.x + child->size->width > size->width) {
                    uint32_t rowElementCount = 0;
                    uint32_t rowHeight = _calculateHorizontalRowHeight(pos.y, rowElementCount);

                    pos.x = 0;
                    pos.y += rowHeight;
                }
            }

            child->position = {
                pos.x + static_cast<int32_t>(child->marginLeft),
                pos.y + static_cast<int32_t>(child->marginTop)
            };
            break;
        }
        case FlowLayout::HorizontalReversed: {
            // Handle the overflow
            if (wrapContents) {
                if (pos.x - static_cast<int32_t>(child->size->width) < 0) {
                    uint32_t rowElementCount = 0;
                    uint32_t rowHeight = _calculateHorizontalRowHeight(pos.y, rowElementCount);

                    pos.x = size->width;
                    pos.y += rowHeight;
                }
            }

            child->position = {
                pos.x - static_cast<int32_t>(child->size->width + child->marginRight),
                pos.y + static_cast<int32_t>(child->marginTop)
            };
            break;
        }
        case FlowLayout::Vertical: {
            child->position = {
                pos.x + static_cast<int32_t>(child->marginLeft),
                pos.y + static_cast<int32_t>(child->marginTop)
            };
            break;
        }
        case FlowLayout::VerticalReversed: {
            child->position = {
                pos.x + static_cast<int32_t>(child->marginLeft),
                pos.y - static_cast<int32_t>(child->size->height + child->marginBottom)
            };
            break;
        }
        default: break;
        }
    }

    void FlowPanel::_calculateNextChildPosition(Position& pos, BaseWidget* child) {
        switch (layout) {
        case FlowLayout::Horizontal: {
            if (!child) {
                // Default position
                pos = Position(0, 0);
                break;
            }

            pos.x += static_cast<int32_t>(
                        child->marginLeft + child->size->width + child->marginRight);
            break;
        }
        case FlowLayout::HorizontalReversed: {
            if (!child) {
                // Default position
                pos = Position(size->width, 0);
                break;
            }

            pos.x -= static_cast<int32_t>(
                        child->marginLeft + child->size->width + child->marginRight);
            break;
        }
        case FlowLayout::Vertical: {
            if (!child) {
                // Default position
                pos = Position(0, 0);
                break;
            }

            pos.y += static_cast<int32_t>(
                        child->marginTop + child->size->height + child->marginBottom);
            break;
        }
        case FlowLayout::VerticalReversed: {
            if (!child) {
                // Default position
                pos = Position(0, size->height);
                break;
            }

            pos.y -= static_cast<int32_t>(
                        child->marginTop + child->size->height + child->marginBottom);
            break;
        }
        default: break;
        }
    }

    void FlowPanel::_justifyRowElementsHorizontally(int32_t positionY, uint32_t elementCount) {
        switch (justifyContent) {
        case JustifyContentType::None: {
            // None is the default type and doesn't require
            // any additional positional adjustments.
            return;
        }
        case JustifyContentType::Fill: {
            // All elements must be packed into the row
            uint32_t elementWidth = size->width / elementCount;

            int32_t newElemPos =
                (layout == Horizontal) ? 0 : static_cast<int32_t>(size->width);

            for (auto& child : d_children) {
                // Ignore elements in a different row
                if (child->position->y - child->marginTop != positionY) {
                    continue;
                }

                child->size->width = elementWidth;

                if (layout == Horizontal) {
                    child->position->x = newElemPos;
                    newElemPos += elementWidth;
                } else if (layout == HorizontalReversed) {
                    child->position->x = newElemPos - child->size->width;
                    newElemPos -= elementWidth;
                }

                child->fireEvent("dynamicallyResized", Event::empty);
            }
            break;
        }
        case JustifyContentType::Center: {
            // All elements must be packed into the row
            // but also packed into the center.
            std::vector<BaseWidget*> affectedElements;

            // Get a list of widgets who are in that row
            // and calculate the sum of all element widths.
            uint32_t totalElemWidth = 0;

            for (auto& child : d_children) {
                // Ignore elements in a different row
                if (child->position->y - child->marginTop != positionY) {
                    continue;
                }

                auto totalChildWidth = child->marginLeft + child->size->width + child->marginRight;

                if (totalElemWidth + totalChildWidth <= size->width) {
                    totalElemWidth += totalChildWidth;
                    affectedElements.push_back(child.get());
                }
            }

            // Calculate the free space left
            uint32_t freeSpace = 0;
            if (static_cast<int32_t>(size->width) - static_cast<int32_t>(totalElemWidth) > 0) {
                freeSpace = (size->width - totalElemWidth) / 2;
            }

            // Adjust the positions of each affected element in the row
            if (layout == Horizontal) {
                int32_t newElemPos = static_cast<int32_t>(freeSpace);
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos + elem->marginLeft;
                    newElemPos += elem->marginLeft + elem->size->width + elem->marginRight;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width - freeSpace);
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos - static_cast<int32_t>(
                                                        elem->size->width + elem->marginRight);

                    newElemPos -= static_cast<int32_t>(
                                    elem->marginLeft + elem->size->width + elem->marginRight);
                }
            }
            break;
        }
        case JustifyContentType::SpaceBetween: {
            std::vector<BaseWidget*> affectedElements;

            // Get a list of widgets who are in that row
            // and calculate the sum of all element widths.
            uint32_t totalElemWidth = 0;

            for (auto& child : d_children) {
                // Ignore elements in a different row
                if (child->position->y - child->marginTop != positionY) {
                    continue;
                }

                auto totalChildWidth = child->marginLeft + child->size->width + child->marginRight;

                if (totalElemWidth + totalChildWidth <= size->width) {
                    totalElemWidth += totalChildWidth;
                    affectedElements.push_back(child.get());
                }
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->width) - static_cast<int32_t>(totalElemWidth) > 0) {
                totalFreeSpace = size->width - totalElemWidth;
            }

            // Calculate the space between elements
            uint32_t spaceBetweenElements = 0;
            if (affectedElements.size() > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(affectedElements.size()) - 1);
            }

            // Adjust the positions of each affected element in the row
            if (layout == Horizontal) {
                int32_t newElemPos = 0;
                for (auto& elem : affectedElements) {
                    auto totalElemWidth = elem->marginLeft + elem->size->width + elem->marginRight;

                    elem->position->x = newElemPos + elem->marginLeft;
                    newElemPos += totalElemWidth + spaceBetweenElements;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width);
                for (auto& elem : affectedElements) {
                    auto totalElemWidth = elem->marginLeft + elem->size->width + elem->marginRight;

                    elem->position->x =
                        newElemPos - static_cast<int32_t>(elem->size->width + elem->marginRight);

                    newElemPos -= static_cast<int32_t>(totalElemWidth) + spaceBetweenElements;
                }
            }
            break;
        }
        case JustifyContentType::SpaceAround: {
            std::vector<BaseWidget*> affectedElements;

            // Get a list of widgets who are in that row
            // and calculate the sum of all element widths.
            uint32_t totalElemWidth = 0;

            for (auto& child : d_children) {
                // Ignore elements in a different row
                if (child->position->y - child->marginTop != positionY) {
                    continue;
                }

                auto totalChildWidth = child->marginLeft + child->size->width + child->marginRight;

                if (totalElemWidth + totalChildWidth <= size->width) {
                    totalElemWidth += totalChildWidth;
                    affectedElements.push_back(child.get());
                }
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->width) - static_cast<int32_t>(totalElemWidth) > 0) {
                totalFreeSpace = size->width - totalElemWidth;
            }

            // Calculate the space between elements
            uint32_t spaceBetweenElements = 0;
            if (affectedElements.size() > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(affectedElements.size()) + 1);
            }

            // Adjust the positions of each affected element in the row
            if (layout == Horizontal) {
                int32_t newElemPos = spaceBetweenElements;
                for (auto& elem : affectedElements) {
                    auto totalElemWidth = elem->marginLeft + elem->size->width + elem->marginRight;

                    elem->position->x = newElemPos + elem->marginLeft;
                    newElemPos += totalElemWidth + spaceBetweenElements;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width) - spaceBetweenElements;
                for (auto& elem : affectedElements) {
                    auto totalElemWidth = elem->marginLeft + elem->size->width + elem->marginRight;

                    elem->position->x =
                        newElemPos - static_cast<int32_t>(elem->size->width + elem->marginRight);

                    newElemPos -= static_cast<int32_t>(totalElemWidth) + spaceBetweenElements;
                }
            }
            break;
        }
        default: break;
        }
    }

    void FlowPanel::_justifyElementsVertically() {
        switch (justifyContent) {
        case JustifyContentType::None: {
            // None is the default type and doesn't require
            // any additional positional adjustments.
            return;
        }
        case JustifyContentType::Fill: {
            // All elements must be packed into the vertical column
            uint32_t elementHeight = size->height / static_cast<uint32_t>(d_children.size());

            int32_t newElemPos =
                (layout == Vertical) ? 0 : static_cast<int32_t>(size->height);

            for (auto& child : d_children) {
                auto childElementHeight = elementHeight - child->marginTop - child->marginBottom;
                child->size->height = childElementHeight;

                if (layout == Vertical) {
                    child->position->y = newElemPos + child->marginTop;
                    newElemPos += childElementHeight + child->marginTop + child->marginBottom;
                } else if (layout == VerticalReversed) {
                    child->position->y = newElemPos - child->size->height - child->marginBottom;
                    newElemPos -= (childElementHeight + child->marginTop + child->marginBottom);
                }

                child->fireEvent("dynamicallyResized", Event::empty);
            }
            break;
        }
        case JustifyContentType::Center: {
            // Calculate the sum of all element heights.
            uint32_t totalChildHeight = 0;

            for (auto& child : d_children) {
                totalChildHeight += child->marginTop + child->size->height + child->marginBottom;
            }

            // Calculate the free space left
            uint32_t freeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalChildHeight) > 0) {
                freeSpace = (size->height - totalChildHeight) / 2;
            }

            // Adjust the positions of each affected element in the column
            if (layout == Vertical) {
                int32_t newElemPos = static_cast<int32_t>(freeSpace);
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y = newElemPos + elem->marginTop;
                    newElemPos += totalElemHeight;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height - freeSpace);
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y =
                        newElemPos - static_cast<int32_t>(elem->size->height + elem->marginBottom);

                    newElemPos -= static_cast<int32_t>(totalElemHeight);
                }
            }
            break;
        }
        case JustifyContentType::SpaceBetween: {
            // Calculate the sum of all element heights.
            uint32_t totalChildHeight = 0;

            for (auto& child : d_children) {
                totalChildHeight += child->marginTop + child->size->height + child->marginBottom;
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalChildHeight) > 0) {
                totalFreeSpace = size->height - totalChildHeight;
            }

            // Calculate the space between elements
            uint32_t spaceBetweenElements = 0;
            if (d_children.size() > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(d_children.size()) - 1);
            }

            // Adjust the positions of each affected element in the column
            if (layout == Vertical) {
                int32_t newElemPos = 0;
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y = newElemPos + elem->marginTop;
                    newElemPos += totalElemHeight + spaceBetweenElements;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height);
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y =
                        newElemPos - static_cast<int32_t>(elem->size->height + elem->marginBottom);

                    newElemPos -= static_cast<int32_t>(totalElemHeight) + spaceBetweenElements;
                }
            }
            break;
        }
        case JustifyContentType::SpaceAround: {
            // Calculate the sum of all element heights.
            uint32_t totalChildHeight = 0;

            for (auto& child : d_children) {
                totalChildHeight += child->marginTop + child->size->height + child->marginBottom;
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalChildHeight) > 0) {
                totalFreeSpace = size->height - totalChildHeight;
            }

            // Calculate the space between elements
            uint32_t spaceBetweenElements = 0;
            if (d_children.size() > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(d_children.size()) + 1);
            }

            // Adjust the positions of each affected element in the row
            if (layout == Vertical) {
                int32_t newElemPos = spaceBetweenElements;
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y = newElemPos + elem->marginTop;
                    newElemPos += totalElemHeight + spaceBetweenElements;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height) - spaceBetweenElements;
                for (auto& elem : d_children) {
                    auto totalElemHeight =
                        elem->marginTop + elem->size->height + elem->marginBottom;

                    elem->position->y =
                        newElemPos - static_cast<int32_t>(elem->size->height + elem->marginBottom);

                    newElemPos -= static_cast<int32_t>(totalElemHeight) + spaceBetweenElements;
                }
            }
            break;
        }
        default: break;
        }
    }
} // namespace mc
