#include "Layout.h"
#include "../BaseWidget.h"

namespace mc {
    void Layout::setWidth(uint32_t width) {
        d_width = width;
    }

    void Layout::setHeight(uint32_t height) {
        d_height = height;
    }

    void Layout::setSize(uint32_t width, uint32_t height) {
        d_width = width;
        d_height = height;
    }

    Position& Layout::getCellPosition() {
        return d_nextAvailableCellPosition;
    }

    void Layout::positionChildren(std::vector<Shared<BaseWidget>>& children) {
        d_nextAvailableCellPosition = resetFirstCellPosition();

        for (auto& child : children) {
            auto& cellPosition = getCellPosition();

            auto position = calculateChildPosition(child.get());
            auto size = calculateChildSize(child.get());

            child->position = position;
            child->width = size.width;
            child->height = size.height;

            advanceCellPosition(d_nextAvailableCellPosition, child.get());
        }
    }
} // namespace mc
