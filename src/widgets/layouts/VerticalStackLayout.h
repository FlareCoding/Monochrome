#pragma once
#include "Layout.h"

namespace mc {
class VerticalStackLayout : public Layout {
protected:
    // Inherited via Layout
    Position resetFirstCellPosition() const override;
    void advanceCellPosition(Position& cellPosition, BaseWidget* widget) const override;
    Position calculateChildPosition(BaseWidget* child) override;
    Size calculateChildSize(BaseWidget* child) override;
};
} // namespace mc
