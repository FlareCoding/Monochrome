#include "VerticalStackLayout.h"
#include "../BaseWidget.h"

namespace mc {
    Position VerticalStackLayout::resetFirstCellPosition() const {
        return { 0, 0 };
    }

    void VerticalStackLayout::advanceCellPosition(
        Position& cellPosition,
        BaseWidget* widget
    ) const {
        //cellPosition.y += widget->getClientSize().height;
    }

    Position VerticalStackLayout::calculateChildPosition(BaseWidget* child) {
        Position result = Position(0, 0);

        //auto& cellPosition = getCellPosition();
        //auto widgetSize = child->getClientSize();

        //// Automatically determine the vertical position
        //result.y = cellPosition.y + static_cast<int32_t>(child->marginTop);

        //switch (child->horizontalAlignment) {
        //case HorizontalAlignment::HALeft: {
        //    result.x = static_cast<int32_t>(child->marginLeft);
        //    break;
        //}
        //case HorizontalAlignment::HARight: {
        //    result.x = static_cast<int32_t>(d_width - child->marginRight - child->width);
        //    break;
        //}
        //case HorizontalAlignment::HACenter: {
        //    result.x = static_cast<int32_t>((d_width / 2) - (widgetSize.width / 2));
        //    break;
        //}
        //case HorizontalAlignment::HAFill: {
        //    result.x = static_cast<int32_t>(child->marginLeft);
        //    break;
        //}
        //default: break;
        //}

        return result;
    }

    Size VerticalStackLayout::calculateChildSize(BaseWidget* child) {
        //Size result = { child->width, child->height };

        //switch (child->horizontalAlignment) {
        //case HorizontalAlignment::HAFill: {
        //    // Stretch the size of the child to fit into the entire cell
        //    auto childMargins = child->marginLeft + child->marginRight;

        //    result.width = d_width - childMargins;

        //    // Sanity checking child's new width
        //    if (result.width > child->maxWidth) {
        //        result.width = child->maxWidth;
        //    } else if (result.width < child->minWidth) {
        //        result.width = child->minWidth;
        //    }
        //    break;
        //}
        //default: break;
        //}

        Size result = Size(0, 0);
        return result;
    }
} // namespace mc
