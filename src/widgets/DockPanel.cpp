#include "DockPanel.h"
#include "visuals/RectVisual.h"

namespace mc {
    DockPanel::DockPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size DockPanel::_measureSize() {
        return Size(fixedWidth, fixedHeight);
    }

    void DockPanel::_onArrangeChildren() {
        auto contentSize = getComputedSize();
        Frame availableSpace = Frame(
            Position(0, 0),
            Size(contentSize.width, contentSize.height)
        );

        for (auto& child : _getChildren()) {
            // Checking if there is no more available space left
            if (availableSpace.size.width == 0 &&
                availableSpace.size.height == 0
            ) {
                break;
            }

            // Finalize child's size and position
            _finalizeChild(child, availableSpace);
        }
    }

    void DockPanel::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void DockPanel::_setupProperties() {
        backgroundColor = Color(); // default value
        backgroundColor.forwardEmittedEvents(this);

        cursorType = CursorType::Arrow;
    }

    void DockPanel::_finalizeChild(Shared<BaseWidget> child, Frame& availableSpace) {
        // Get child's desired size
        auto childDesiredSize = child->getDesiredSize();

        // Calculate the final position and size for each child
        switch (child->dockAnchor) {
        case DockAnchor::Top: {
            auto finalChildSize = Size(
                availableSpace.size.width - child->marginLeft - child->marginRight,
                childDesiredSize.height
            );

            child->position = Position(
                child->marginLeft + availableSpace.position.x,
                child->marginTop + availableSpace.position.y
            );

            child->setComputedSize(finalChildSize);

            // Adjusting the available space left
            auto childSizeWithMargins = child->getComputedSizeWithMargins();
            availableSpace.position.y += childSizeWithMargins.height;
            availableSpace.size.height -= childSizeWithMargins.height;
            break;
        }
        case DockAnchor::Bottom: {
            auto finalChildSize = Size(
                availableSpace.size.width - child->marginLeft - child->marginRight,
                childDesiredSize.height
            );

            child->position = Position(
                child->marginLeft + availableSpace.position.x,

                availableSpace.position.y + availableSpace.size.height -
                                finalChildSize.height - child->marginBottom
            );

            child->setComputedSize(finalChildSize);

            // Adjusting the available space left
            auto childSizeWithMargins = child->getComputedSizeWithMargins();
            availableSpace.size.height -= childSizeWithMargins.height;
            break;
        }
        case DockAnchor::Left: {
            auto finalChildSize = Size(
                childDesiredSize.width,
                availableSpace.size.height - child->marginTop - child->marginBottom
            );

            child->position = Position(
                child->marginLeft + availableSpace.position.x,
                child->marginTop + availableSpace.position.y
            );

            child->setComputedSize(finalChildSize);

            // Adjusting the available space left
            auto childSizeWithMargins = child->getComputedSizeWithMargins();
            availableSpace.position.x += childSizeWithMargins.width;
            availableSpace.size.width -= childSizeWithMargins.width;
            break;
        }
        case DockAnchor::Right: {
            auto finalChildSize = Size(
                childDesiredSize.width,
                availableSpace.size.height - child->marginTop - child->marginBottom
            );

            child->position = Position(
                availableSpace.position.x + availableSpace.size.width -
                            finalChildSize.width - child->marginRight,

                child->marginTop + availableSpace.position.y
            );

            child->setComputedSize(finalChildSize);

            // Adjusting the available space left
            auto childSizeWithMargins = child->getComputedSizeWithMargins();
            availableSpace.size.width -= childSizeWithMargins.width;
            break;
        }
        case DockAnchor::Fill: {
            auto finalChildSize = Size(
                availableSpace.size.width - child->marginLeft - child->marginRight,
                availableSpace.size.height - child->marginTop - child->marginBottom
            );

            child->position = Position(
                availableSpace.position.x + child->marginLeft,
                availableSpace.position.y + child->marginTop
            );

            child->setComputedSize(finalChildSize);

            // Adjusting the available space left
            auto childSizeWithMargins = child->getComputedSizeWithMargins();
            availableSpace.position.x += childSizeWithMargins.width;
            availableSpace.position.y += childSizeWithMargins.height;
            availableSpace.size.width -= childSizeWithMargins.width;
            availableSpace.size.height -= childSizeWithMargins.width;
            break;
        }
        default: break;
        }
    }
} // namespace mc
