#pragma once
#include "../Layer.h"
#include <vector>

namespace mc {
class BaseWidget;

class Layout {
public:
    virtual ~Layout() = default;

    void positionChildren(std::vector<Shared<BaseWidget>>& children);

    void setWidth(uint32_t width);
    void setHeight(uint32_t height);
    void setSize(uint32_t width, uint32_t height);

protected:
    uint32_t d_width = 0, d_height = 0;

    Position& getCellPosition();

    virtual Position resetFirstCellPosition() const = 0;
    virtual void advanceCellPosition(Position& cellPosition, BaseWidget* widget) const = 0;
    virtual Position calculateChildPosition(BaseWidget* child) = 0;
    virtual Size calculateChildSize(BaseWidget* child) = 0;

private:
    Position d_nextAvailableCellPosition = { 0, 0 };
};
} // namespace mc
