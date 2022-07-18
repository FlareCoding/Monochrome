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
    }

    void FlowPanel::_calculateChildrenDynamicPosition() {
        Frame availableInsertionFrame;
        _calculateNextChildPosition(availableInsertionFrame.position, nullptr);

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
            } else {
                // Logic gets simplified a lot since there
                // is no element wrapping in vertical layouts.
                _justifyElementsVertically();
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
            if (child->position->y != targetRowPositionY) {
                continue;
            }

            // Track the element
            ++elementCount;

            // Track the greatest element height
            if (child->size->height > rowHeight) {
                rowHeight = child->size->height;
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

            child->position = pos;
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
                pos.x - static_cast<int32_t>(child->size->width),
                pos.y
            };
            break;
        }
        case FlowLayout::Vertical: {
            child->position = pos;
            break;
        }
        case FlowLayout::VerticalReversed: {
            child->position = {
                pos.x,
                pos.y - static_cast<int32_t>(child->size->height)
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

            pos.x += child->size->width;
            break;
        }
        case FlowLayout::HorizontalReversed: {
            if (!child) {
                // Default position
                pos = Position(size->width, 0);
                break;
            }

            pos.x -= static_cast<int32_t>(child->size->width);
            break;
        }
        case FlowLayout::Vertical: {
            if (!child) {
                // Default position
                pos = Position(0, 0);
                break;
            }

            pos.y += child->size->height;
            break;
        }
        case FlowLayout::VerticalReversed: {
            if (!child) {
                // Default position
                pos = Position(0, size->height);
                break;
            }

            pos.y -= static_cast<int32_t>(child->size->height);
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
                if (child->position->y != positionY) {
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
                if (child->position->y != positionY) {
                    continue;
                }

                if (totalElemWidth + child->size->width <= size->width) {
                    totalElemWidth += child->size->width;
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
                    elem->position->x = newElemPos;
                    newElemPos += elem->size->width;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width - freeSpace);
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos - static_cast<int32_t>(elem->size->width);
                    newElemPos -= static_cast<int32_t>(elem->size->width);
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
                if (child->position->y != positionY) {
                    continue;
                }

                if (totalElemWidth + child->size->width <= size->width) {
                    totalElemWidth += child->size->width;
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
            if (elementCount > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(affectedElements.size()) - 1);
            }

            // Adjust the positions of each affected element in the row
            if (layout == Horizontal) {
                int32_t newElemPos = 0;
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos;
                    newElemPos += elem->size->width + spaceBetweenElements;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width);
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos - static_cast<int32_t>(elem->size->width);
                    newElemPos -= static_cast<int32_t>(elem->size->width) + spaceBetweenElements;
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
                if (child->position->y != positionY) {
                    continue;
                }

                if (totalElemWidth + child->size->width <= size->width) {
                    totalElemWidth += child->size->width;
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
            if (elementCount > 1) {
                spaceBetweenElements =
                    totalFreeSpace / (static_cast<uint32_t>(affectedElements.size()) + 1);
            }

            // Adjust the positions of each affected element in the row
            if (layout == Horizontal) {
                int32_t newElemPos = spaceBetweenElements;
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos;
                    newElemPos += elem->size->width + spaceBetweenElements;
                }
            } else if (layout == HorizontalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->width) - spaceBetweenElements;
                for (auto& elem : affectedElements) {
                    elem->position->x = newElemPos - static_cast<int32_t>(elem->size->width);
                    newElemPos -= static_cast<int32_t>(elem->size->width) + spaceBetweenElements;
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
                child->size->height = elementHeight;

                if (layout == Vertical) {
                    child->position->y = newElemPos;
                    newElemPos += elementHeight;
                } else if (layout == VerticalReversed) {
                    child->position->y = newElemPos - child->size->height;
                    newElemPos -= elementHeight;
                }
            }
            break;
        }
        case JustifyContentType::Center: {
            // Calculate the sum of all element heights.
            uint32_t totalElemHeight = 0;

            for (auto& child : d_children) {
                totalElemHeight += child->size->height;
            }

            // Calculate the free space left
            uint32_t freeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalElemHeight) > 0) {
                freeSpace = (size->height - totalElemHeight) / 2;
            }

            // Adjust the positions of each affected element in the column
            if (layout == Vertical) {
                int32_t newElemPos = static_cast<int32_t>(freeSpace);
                for (auto& elem : d_children) {
                    elem->position->y = newElemPos;
                    newElemPos += elem->size->height;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height - freeSpace);
                for (auto& elem : d_children) {
                    elem->position->y = newElemPos - static_cast<int32_t>(elem->size->height);
                    newElemPos -= static_cast<int32_t>(elem->size->height);
                }
            }
            break;
        }
        case JustifyContentType::SpaceBetween: {
            // Calculate the sum of all element heights.
            uint32_t totalElemHeight = 0;

            for (auto& child : d_children) {
                totalElemHeight += child->size->height;
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalElemHeight) > 0) {
                totalFreeSpace = size->height - totalElemHeight;
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
                    elem->position->y = newElemPos;
                    newElemPos += elem->size->height + spaceBetweenElements;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height);
                for (auto& elem : d_children) {
                    elem->position->y = newElemPos - static_cast<int32_t>(elem->size->height);
                    newElemPos -= static_cast<int32_t>(elem->size->height) + spaceBetweenElements;
                }
            }
            break;
        }
        case JustifyContentType::SpaceAround: {
            // Calculate the sum of all element heights.
            uint32_t totalElemHeight = 0;

            for (auto& child : d_children) {
                totalElemHeight += child->size->height;
            }

            // Calculate the free space left
            uint32_t totalFreeSpace = 0;
            if (static_cast<int32_t>(size->height) - static_cast<int32_t>(totalElemHeight) > 0) {
                totalFreeSpace = size->height - totalElemHeight;
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
                    elem->position->y = newElemPos;
                    newElemPos += elem->size->height + spaceBetweenElements;
                }
            } else if (layout == VerticalReversed) {
                int32_t newElemPos = static_cast<int32_t>(size->height) - spaceBetweenElements;
                for (auto& elem : d_children) {
                    elem->position->y = newElemPos - static_cast<int32_t>(elem->size->height);
                    newElemPos -= static_cast<int32_t>(elem->size->height) + spaceBetweenElements;
                }
            }
            break;
        }
        default: break;
        }
    }
} // namespace mc
