#include "StackPanel.h"
#include "visuals/Visuals.h"

namespace mc {
    StackPanel::StackPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size StackPanel::_measureSize() {
        Position availablePos = Position(0, 0);
        Size contentSize = Size(0, 0);

        for (auto& child : _getChildren()) {
            auto childSizeWithMargins = child->getDesiredSizeWithMargins();

            if (orientation == Vertical) {
                availablePos.y += childSizeWithMargins.height;

                // Keeping track of content size
                if (contentSize.width < childSizeWithMargins.width) {
                    contentSize.width = childSizeWithMargins.width;
                }

                contentSize.height += childSizeWithMargins.height;
            } else {
                availablePos.x += childSizeWithMargins.width;

                // Keeping track of content size
                if (contentSize.height < childSizeWithMargins.height) {
                    contentSize.height = childSizeWithMargins.height;
                }

                contentSize.width += childSizeWithMargins.width;
            }
        }

        return contentSize;
    }

    void StackPanel::_onArrangeChildren() {
        Position childSlotPosition = Position(0, 0);
        auto contentSize = getComputedSize();

        for (auto& child : _getChildren()) {
            // Calculate the final position and size for each child
            _finalizeChild(child, contentSize, childSlotPosition);

            // Advance the available insertion position
            auto childSizeWithMargins = child->getComputedSizeWithMargins();

            if (orientation == Vertical) {
                childSlotPosition.y += childSizeWithMargins.height;
            } else {
                childSlotPosition.x += childSizeWithMargins.width;
            }
        }
    }

    void StackPanel::_finalizeChild(
        Shared<BaseWidget> child,
        const Size& contentSize,
        const Position& childSlotPosition
    ) {
        auto finalSize = child->getDesiredSize();
        auto finalPosition = Position(0, 0);

        if (orientation == Vertical) {
            finalPosition.y = static_cast<int32_t>(child->marginTop) + childSlotPosition.y;

            switch (child->horizontalAlignment) {
            case HALeft: {
                finalPosition.x = static_cast<int32_t>(child->marginLeft);
                break;
            }
            case HARight: {
                finalPosition.x =
                    static_cast<int32_t>(contentSize.width - finalSize.width - child->marginRight);
                break;
            }
            case HACenter: {
                finalPosition.x = static_cast<int32_t>(
                    contentSize.width / 2 - finalSize.width / 2
                );
                break;
            }
            case HAFill: {
                finalPosition.x = static_cast<int32_t>(child->marginLeft);
                finalSize.width = contentSize.width - child->marginLeft - child->marginRight;
                break;
            }
            default: break;
            }
        } else {
            finalPosition.x = static_cast<int32_t>(child->marginLeft) + childSlotPosition.x;

            switch (child->verticalAlignment) {
            case VATop: {
                finalPosition.y = static_cast<int32_t>(child->marginTop);
                break;
            }
            case VABottom: {
                finalPosition.y = static_cast<int32_t>(
                    contentSize.height - finalSize.height - child->marginBottom
                );
                break;
            }
            case VACenter: {
                finalPosition.y = static_cast<int32_t>(
                    contentSize.height / 2 - finalSize.height / 2
                );
                break;
            }
            case VAFill: {
                finalPosition.y = static_cast<int32_t>(child->marginTop);
                finalSize.height = contentSize.height - child->marginTop - child->marginBottom;
                break;
            }
            default: break;
            }
        }

        child->position = finalPosition;
        child->setComputedSize(finalSize);
    }

    void StackPanel::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void StackPanel::_setupProperties() {
        backgroundColor = Color::lightGray;
        backgroundColor.forwardEmittedEvents(this);

        cornerRadius = 2;
        cornerRadius.forwardEmittedEvents(this);

        orientation = Vertical;
        orientation.forwardEmittedEvents(this);
        orientation.on("propertyChanged", [this](Shared<Event> e) {
            signalLayoutChanged();
        });
    }
} // namespace mc
