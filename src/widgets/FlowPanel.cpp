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

            CORE_ASSERT(false, "Vertical layout not supported (yet)");
            break;
        }
        case FlowLayout::VerticalReversed: {
            CORE_ASSERT(false, "VerticalReversed layout not supported (yet)");
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

            CORE_ASSERT(false, "Vertical layout not supported (yet)");
            break;
        }
        case FlowLayout::VerticalReversed: {
            if (!child) {
                // Default position
                pos = Position(0, size->height);
                break;
            }

            CORE_ASSERT(false, "VerticalReversed layout not supported (yet)");
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

            for (auto& child : d_children) {
                // Ignore elements in a different row
                if (child->position->y != positionY) {
                    continue;
                }

                child->size->width = elementWidth;
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
            int32_t newElemPos = static_cast<int32_t>(freeSpace);
            for (auto& elem : affectedElements) {
                elem->position->x = newElemPos;
                newElemPos += elem->size->width;
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
            int32_t newElemPos = 0;
            for (auto& elem : affectedElements) {
                elem->position->x = newElemPos;
                newElemPos += elem->size->width + spaceBetweenElements;
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
            int32_t newElemPos = spaceBetweenElements;
            for (auto& elem : affectedElements) {
                elem->position->x = newElemPos;
                newElemPos += elem->size->width + spaceBetweenElements;
            }
            break;
        }
        default: break;
        }
    }
} // namespace mc