#include "StackPanel.h"
#include "visuals/Visuals.h"
#include "layouts/VerticalStackLayout.h"
#include "layouts/HorizontalStackLayout.h"

namespace mc {
    StackPanel::StackPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size StackPanel::_measureSize() {
        // Measure phase
        for (auto& child : _getChildren()) {
            child->measure();
        }

        Position availablePos = Position(0, 0);
        Size contentSize = Size(0, 0);

        for (auto& child : _getChildren()) {
            auto childSizeWithMargins = child->getDesiredSizeWithMargins();

            if (orientation == Orientaion::Vertical) {
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
        Position availablePos = Position(0, 0);
        auto contentSize = getComputedSize();

        for (auto& child : _getChildren()) {
            auto desiredSize = child->getDesiredSize();

            // Set child's position
            child->position = {
                static_cast<int32_t>(child->marginLeft) + availablePos.x,
                static_cast<int32_t>(child->marginTop) + availablePos.y
            };

            // Calculate the final size for each child
            auto finalSize = desiredSize;

            if (orientation == Orientaion::Vertical) {
                if (child->horizontalAlignment == HorizontalAlignment::HAFill) {
                    finalSize.width = contentSize.width - child->marginLeft - child->marginRight;
                }
            }

            child->setComputedSize(finalSize);

            // Advance the available insertion position
            auto childSizeWithMargins = child->getComputedSizeWithMargins();

            if (orientation == Orientaion::Vertical) {
                availablePos.y += childSizeWithMargins.height;
            } else {
                availablePos.x += childSizeWithMargins.width;
            }
        }
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

        orientation = Orientaion::Vertical;
        orientation.forwardEmittedEvents(this);
    }
} // namespace mc
