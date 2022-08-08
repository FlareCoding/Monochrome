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
        return updateLayout();
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

    Size StackPanel::updateLayout() {
        // Measure phase
        for (auto& child : _getChildren()) {
            child->measure();
        }

        // Arrange phase
        Position availablePos = Position(0, 0);
        Size contentSize = Size(0, 0);

        for (auto& child : _getChildren()) {
            auto finalSize = child->getDesiredSize();

            /*if (orientation == Orientaion::Vertical) {
                if (child->horizontalAlignment == HAFill) {
                    finalSize.width =
                        getComputedSize().width - child->marginLeft - child->marginRight;
                }
            } else {
                if (child->verticalAlignment == VAFill) {
                    finalSize.height =
                        getComputedSize().height - child->marginTop - child->marginBottom;
                }
            }*/

            if (child->fixedWidth != NOT_SET) {
                finalSize.width = child->fixedWidth;
            }

            if (child->fixedHeight != NOT_SET) {
                finalSize.height = child->fixedHeight;
            }

            // Set the child's final computed size
            child->setComputedSize(finalSize);

            // Set child's position
            child->position = {
                static_cast<int32_t>(child->marginLeft) + availablePos.x,
                static_cast<int32_t>(child->marginTop) + availablePos.y
            };

            auto childSizeWithMargins = child->getComputedSizeWithMargins();

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

        if (fixedWidth != NOT_SET) {
            contentSize.width = fixedWidth;
        }

        if (fixedHeight != NOT_SET) {
            contentSize.height = fixedHeight;
        }

        // Min/max width constraints
        if (contentSize.width > maxWidth) {
            contentSize.width = maxWidth;
        } else if (contentSize.width < minWidth) {
            contentSize.width = minWidth;
        }

        // Min/max height constraints
        if (contentSize.height > maxHeight) {
            contentSize.height = maxHeight;
        } else if (contentSize.height < minHeight) {
            contentSize.height = minHeight;
        }

        return contentSize;
    }
} // namespace mc
