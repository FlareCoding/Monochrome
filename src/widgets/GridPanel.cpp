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

        // Other containers in Monochrome measure their size by iterating
        // over their list of children using the _getChildren() function, however
        // the GridPanel should traverse its rows and columns to identify the size
        // of each row and each column.
        // In order to determine that:
        //        - iterating over every column in a row will calculate the row's computed height
        //        - iterating over every row will result in calculating the column's computed width

        // Measure each row's height
        for (size_t rowIdx = 0; rowIdx < d_rowDefs.size(); ++rowIdx) {
            auto& rowDefinition = d_rowDefs[rowIdx];

            // Check if the row has a predetermined fixed size
            if (rowDefinition->fixedHeight != NOT_SET) {
                rowDefinition->_computedRowHeight = rowDefinition->fixedHeight;
            } else {
                // If the row doesn't have a fixed size, calculate it
                // based on min/max bounds and children's desired sizes.
                uint32_t computedRowHeight = _calculateRowHeight(rowIdx);

                // Check the min bound
                if (computedRowHeight < rowDefinition->minHeight) {
                    computedRowHeight = rowDefinition->minHeight;
                }

                // Check the max bound
                if (computedRowHeight > rowDefinition->maxHeight) {
                    computedRowHeight = rowDefinition->maxHeight;
                }

                // Finalize row's height
                rowDefinition->_computedRowHeight = computedRowHeight;
            }

            // Add the row's height to the content height
            contentSize.height += rowDefinition->_computedRowHeight;
        }

        // Measure each row's width
        for (size_t colIdx = 0; colIdx < d_columnDefs.size(); ++colIdx) {
            auto& columnDefinition = d_columnDefs[colIdx];

            // Check if the column has a predetermined fixed size
            if (columnDefinition->fixedWidth != NOT_SET) {
                columnDefinition->_computedColumnWidth = columnDefinition->fixedWidth;
            } else {
                // If the colummn doesn't have a fixed size, calculate it
                // based on min/max bounds and children's desired sizes.
                uint32_t computedColumnWidth = _calculateColumnWidth(colIdx);

                // Check the min bound
                if (computedColumnWidth < columnDefinition->minWidth) {
                    computedColumnWidth = columnDefinition->minWidth;
                }

                // Check the max bound
                if (computedColumnWidth > columnDefinition->maxWidth) {
                    computedColumnWidth = columnDefinition->maxWidth;
                }

                // Finalize column's width
                columnDefinition->_computedColumnWidth = computedColumnWidth;
            }

            // Add the column's width to the content width
            contentSize.width += columnDefinition->_computedColumnWidth;
        }

        return contentSize;
    }

    uint32_t GridPanel::_calculateRowHeight(size_t rowIdx) {
        uint32_t computedRowHeight = 0;

        // Iterate over every column in the row
        for (size_t colIdx = 0; colIdx < d_columnDefs.size(); ++colIdx) {
            GridCell& cell = _getGridCell(rowIdx, colIdx);

            // Skip over empty cells
            if (!cell.content) {
                continue;
            }

            // Get the child widget's desired size at the current cell
            auto childDesiredSize = cell.content->getDesiredSizeWithMargins();

            // If the child's desired size is greater than the currently
            // calculated row size, increase the row size to fit the child.
            if (childDesiredSize.height > computedRowHeight) {
                computedRowHeight = childDesiredSize.height;
            }
        }

        return computedRowHeight;
    }

    uint32_t GridPanel::_calculateColumnWidth(size_t colIdx) {
        uint32_t computedColumnWidth = 0;

        // Iterate over every row in the column
        for (size_t rowIdx = 0; rowIdx < d_rowDefs.size(); ++rowIdx) {
            GridCell& cell = _getGridCell(rowIdx, colIdx);

            // Skip over empty cells
            if (!cell.content) {
                continue;
            }

            // Get the child widget's desired size at the current cell
            auto childDesiredSize = cell.content->getDesiredSizeWithMargins();

            // If the child's desired size is greater than the currently
            // calculated column size, increase the row size to fit the child.
            if (childDesiredSize.width > computedColumnWidth) {
                computedColumnWidth = childDesiredSize.width;
            }
        }

        return computedColumnWidth;
    }

    void GridPanel::_onArrangeChildren() {
        Position childSlotPosition = Position(0, 0);

        // Iterate over every column in each row
        for (size_t rowIdx = 0; rowIdx < d_rowDefs.size(); ++rowIdx) {
            auto& rowDefinition = d_rowDefs[rowIdx];

            // Reset the column position
            childSlotPosition.x = 0;

            for (size_t colIdx = 0; colIdx < d_columnDefs.size(); ++colIdx) {
                auto& columnDefinition = d_columnDefs[colIdx];

                GridCell& cell = _getGridCell(rowIdx, colIdx);
                if (cell.content) {
                    cell.content->position = childSlotPosition;
                    
                    // Adjust the child's size to fit into the cell if needed
                    auto childSize = cell.content->getDesiredSizeWithMargins();

                    if (childSize.width > columnDefinition->_computedColumnWidth) {
                        childSize.width = columnDefinition->_computedColumnWidth;
                    }

                    if (childSize.height > rowDefinition->_computedRowHeight) {
                        childSize.height = rowDefinition->_computedRowHeight;
                    }

                    // Finalize the child's computed size
                    cell.content->setComputedSize(childSize);
                }

                childSlotPosition.x += columnDefinition->_computedColumnWidth;
            }

            childSlotPosition.y += rowDefinition->_computedRowHeight;
        }
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
        std::vector<GridCell> columnsList;
        columnsList.resize(d_columnDefs.size());

        d_gridCells.push_back(columnsList);
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

    void GridPanel::updateLayout() {
        signalLayoutChanged();
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
