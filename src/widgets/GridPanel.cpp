#include "GridPanel.h"
#include "visuals/Visuals.h"

namespace mc {
    GridPanel::GridPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size GridPanel::_measureSize() {
        Position availablePos = Position(0, 0);
        Size contentSize = Size(0, 0);

        for (size_t rowIdx = 0; rowIdx < d_rowDefs.size(); ++rowIdx) {
            // Get the row definition and reset its computed size to 0
            auto& rowDefinition = d_rowDefs[rowIdx];
            rowDefinition->_computedRowHeight = 0;

            for (size_t colIdx = 0; colIdx < d_columnDefs.size(); ++colIdx) {
                // Get the column definition and reset its computed size to 0
                auto& columnDefinition = d_columnDefs[colIdx];
                columnDefinition->_computedColumnWidth = 0;

                // Access the current cell with its content
                GridCell& cell = _getGridCell(rowIdx, colIdx);

                // Get the desired size of the child
                auto childDesiredSize = cell.content->getDesiredSizeWithMargins();

                // Check if the content size is too small for the column
            }
        }

        return contentSize;
    }

    void GridPanel::_onArrangeChildren() {
        Position childSlotPosition = Position(0, 0);
        auto contentSize = getComputedSize();
    }

    void GridPanel::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        cornerRadius.forwardAssignment(&bodyRect->cornerRadius);
        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void GridPanel::_setupProperties() {
        backgroundColor = BaseWidget::getUniversalBackgroundColor();
        handleWidgetVisiblePropertyChange(backgroundColor);

        cornerRadius = 2;
        handleWidgetVisiblePropertyChange(cornerRadius);
    }

    void GridPanel::addRowDefinition(Shared<RowDefinition> rowDef) {
        // Push a new row definition
        d_rowDefs.push_back(rowDef);

        // Create a new row vector to store grid cell objects
        d_gridCells.push_back({});
    }

    void GridPanel::addColumnDefinition(Shared<ColumnDefinition> colDef) {
        // Push a new column definition
        d_columnDefs.push_back(colDef);

        // Create new cells for the new column
        for (size_t row = 0; row < d_rowDefs.size(); ++row) {
            auto& columnList = d_gridCells.at(row);

            // Allocate enough cells for the columns
            columnList.resize(d_columnDefs.size());
        }
    }

    Shared<RowDefinition> GridPanel::getRowDefinition(size_t row) {
        CORE_ASSERT(row < d_rowDefs.size(), "Invalid row requested. Rows available: " +
            std::to_string(d_rowDefs.size()) + "  Row requested: " + std::to_string(row));

        return d_rowDefs.at(row);
    }

    Shared<ColumnDefinition> GridPanel::getColumnDefinition(size_t col) {
        CORE_ASSERT(col < d_columnDefs.size(), "Invalid column requested. Columns available: " +
            std::to_string(d_columnDefs.size()) + "  Column requested: " + std::to_string(col));

        return d_columnDefs.at(col);
    }

    void GridPanel::setContent(Shared<BaseWidget> widget, size_t row, size_t col) {
        GridCell& cell = _getGridCell(row, col);

        cell.content = widget;
        addChild(widget);
    }

    void GridPanel::setContentRowSpan(Shared<BaseWidget> widget, size_t span) {
        GridCell* cell = _findGridCellByContent(widget);

        CORE_ASSERT(cell, "Could not find a grid cell with the specified content widget");

        if (cell->rowSpan != span) {
            cell->rowSpan = span;
            signalLayoutChanged();
        }
    }

    void GridPanel::setContentColumnSpan(Shared<BaseWidget> widget, size_t span) {
        GridCell* cell = _findGridCellByContent(widget);

        CORE_ASSERT(cell, "Could not find a grid cell with the specified content widget");

        if (cell->columnSpan != span) {
            cell->columnSpan = span;
            signalLayoutChanged();
        }
    }

    GridCell& GridPanel::_getGridCell(size_t row, size_t col) {
        // Sanity check the row
        CORE_ASSERT(row < d_rowDefs.size(), "Invalid row requested. Rows available: " +
            std::to_string(d_rowDefs.size()) + "  Row requested: " + std::to_string(row));
        
        // Sanity check the column
        CORE_ASSERT(col < d_columnDefs.size(), "Invalid column requested. Columns available: " +
            std::to_string(d_columnDefs.size()) + "  Column requested: " + std::to_string(col));

        return d_gridCells.at(row).at(col);
    }

    GridCell* GridPanel::_findGridCellByContent(Shared<BaseWidget>& content) {
        for (auto& columnList : d_gridCells) {
            for (auto& cell : columnList) {
                if (cell.content->getID() == content->getID()) {
                    return &cell;
                }
            }
        }

        return nullptr;
    }
} // namespace mc
