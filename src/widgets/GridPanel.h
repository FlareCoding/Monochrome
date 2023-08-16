#pragma once
#include "BaseWidget.h"
#include "Image.h"

namespace mc {
struct RowDefinition {
    uint32_t height    = NOT_SET;
    uint32_t minHeight = NOT_SET;
    uint32_t maxHeight = std::numeric_limits<uint32_t>::max();

    uint32_t _computedRowHeight = NOT_SET;
};

struct ColumnDefinition {
    uint32_t width    = NOT_SET;
    uint32_t minWidth = NOT_SET;
    uint32_t maxWidth = std::numeric_limits<uint32_t>::max();

    uint32_t _computedColumnWidth = NOT_SET;
};

struct GridCell {
    Shared<BaseWidget>  content     = nullptr;
    size_t              rowSpan     = 1;
    size_t              columnSpan  = 1;
};

class GridPanel : public BaseContainerWidget {
public:
    GridPanel();

    std::string getWidgetName() const override { return "GridPanel"; }

    // Radius of the panel's corners
    PropertyObserver<uint32_t>      cornerRadius;

    // Color of the panel's body
    PropertyObserver<Color>         backgroundColor;

    inline size_t getRowCount() const { return d_rowDefs.size(); }
    inline size_t getColumnCount() const { return d_columnDefs.size(); }

    void addRowDefinition(Shared<RowDefinition> rowDef);
    void addColumnDefinition(Shared<ColumnDefinition> colDef);

    Shared<RowDefinition> getRowDefinition(size_t row);
    Shared<ColumnDefinition> getColumnDefinition(size_t col);

    void setContent(Shared<BaseWidget> widget, size_t row, size_t col);

    void setContentRowSpan(Shared<BaseWidget> widget, size_t span);
    void setContentColumnSpan(Shared<BaseWidget> widget, size_t span);

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    GridCell& _getGridCell(size_t row, size_t col);
    GridCell* _findGridCellByContent(Shared<BaseWidget>& content);

private:
    std::vector<Shared<RowDefinition>>      d_rowDefs;
    std::vector<Shared<ColumnDefinition>>   d_columnDefs;

    // Format: [row][column]
    std::vector<std::vector<GridCell>> d_gridCells;
};
} // namespace mc
