#pragma once
#include "BaseWidget.h"
#include "Button.h"

namespace mc {
class SplitterPanel : public BaseContainerWidget {
public:
    SplitterPanel();

    // Color of the panel's body
    PropertyObserver<Color> backgroundColor;

    // @brief Color of the divider lines between each section
    PropertyObserver<Color> dividerColor;

    // @brief Indicates whether or not the
    // sections can be dragged and adjusted.
    // *NOTE: NOT YET IMPLEMENTED
    PropertyObserver<bool> movableDividers;

    // @brief Thickness of dividing lines between sections.
    PropertyObserver<uint32_t> dividerSize;

    // Direction in which the content
    // will be layed out inside the panel.
    PropertyObserver<Orientaion> orientation;

    //
    // Specifies how big each section should
    // be compared to other sections.
    //
    // Example: "1 3 1" for three sections
    // with middle section being the largest.
    //
    // *Note* use empty string for equal weights
    PropertyObserver<std::string> sectionWeights;

protected:
    Size _measureSize() override;
    void _onArrangeChildren() override;

private:
    void _createVisuals();
    void _setupProperties();

    void _createNecessaryDividerVisuals();
    void _resetDividers();

private:
    std::vector<Shared<VisualElement>> d_dividers;
};
} // namespace mc
