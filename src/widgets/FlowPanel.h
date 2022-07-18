#pragma once
#include "Panel.h"

namespace mc {
enum FlowLayout {
    // Elements will be positioned horizontally left to right
    Horizontal,

    // Elements will be positioned horizontally right to left
    HorizontalReversed,

    // Elements will be positioned vertically top to bottom
    Vertical,

    // Elements will be positioned vertically bottom to top
    VerticalReversed
};

enum JustifyContentType {
    // Elements will be positioned directly next to each other
    None,

    // Elements will be spread out as much as
    // possible leaving even spaces between each other.
    SpaceBetween,

    // Elements will be positioned with space evenly distributed around them
    SpaceAround,

    // Elements will be placed in the center directly next to each other
    Center,

    // Elements will be stretched out to fill the available space
    Fill
};

class FlowPanel : public Panel {
public:
    FlowPanel();

    // Specifies in which direction and
    // order the elements should be layed out.
    PropertyObserver<FlowLayout>            layout;

    // Specifies what the spacing between elements should be like
    PropertyObserver<JustifyContentType>    justifyContent;

    // If set to true, when an element can't fit into the current row,
    // the element will be positioned onto the next available row.
    // *Only applies to horizontal layouts*
    PropertyObserver<bool>                  wrapContents;

    // Determines if elements should stretch to fill the panel vertically
    // in a horizontal layout and horizontally in a vertical layout.
    PropertyObserver<bool>                  stretchContents;

    // If true, the panel will resize itself to the bounds of the content.
    // The user has to specify only the width for horizontal layouts and only the height
    // for vertical layouts to create the appropriate bounds for layout configurations.
    PropertyObserver<bool>                  autoSize;

private:
    void _setupFlowpanelProperties();
    void _setupEventHandlers();

    void _calculateChildrenDynamicPosition();
    uint32_t _calculateHorizontalRowHeight(int32_t targetRowPositionY, uint32_t& elementCount);
    void _calculateCurrentChildPosition(Position& pos, BaseWidget* child);
    void _calculateNextChildPosition(Position& pos, BaseWidget* child);

    void _justifyRowElementsHorizontally(int32_t positionY, uint32_t elementCount);
    void _justifyElementsVertically();

private:
    // rowPositionY -> { totalRowElementWidth, rowElements }
    std::map<uint32_t, std::pair<uint32_t, std::vector<BaseWidget*>>> d_rowElementsMap;
};
} // namespace mc
