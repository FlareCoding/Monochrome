#include "HorizontalStackLayout.h"
#include "../BaseWidget.h"

namespace mc {
    Position HorizontalStackLayout::resetFirstCellPosition() const {
        return { 0, 0 };
    }

    void HorizontalStackLayout::advanceCellPosition(
        Position& cellPosition,
        BaseWidget* widget
    ) const {
        cellPosition.x += widget->getClientSize().width;
    }

    Position HorizontalStackLayout::calculateChildPosition(BaseWidget* child) {
        Position result;

        auto& cellPosition = getCellPosition();
        auto widgetSize = child->getClientSize();

        // Automatically determine the vertical position
        result.x = cellPosition.x + static_cast<int32_t>(child->marginLeft);

        switch (child->horizontalAlignment) {
        case VerticalAlignment::VATop: {
            result.y = static_cast<int32_t>(child->marginTop);
            break;
        }
        case VerticalAlignment::VABottom: {
            result.y = static_cast<int32_t>(d_height - child->marginBottom - child->height);
            break;
        }
        case VerticalAlignment::VACenter: {
            result.y = static_cast<int32_t>((d_height / 2) - (widgetSize.height / 2));
            break;
        }
        case VerticalAlignment::VAFill: {
            result.y = static_cast<int32_t>(child->marginTop);
            break;
        }
        default: break;
        }

        return result;
    }

    Size HorizontalStackLayout::calculateChildSize(BaseWidget* child) {
        Size result = { child->width, child->height };

        switch (child->horizontalAlignment) {
        case VerticalAlignment::VAFill: {
            // Stretch the size of the child to fit into the entire cell
            auto childMargins = child->marginTop + child->marginBottom;

            result.height = d_height - childMargins;

            // Sanity checking child's new width
            if (result.height > child->maxHeight) {
                result.height = child->maxHeight;
            }
            else if (result.height < child->minHeight) {
                result.height = child->minHeight;
            }
            break;
        }
        default: break;
        }

        return result;
    }
} // namespace mc
