#include "SplitterPanel.h"
#include "visuals/Visuals.h"
#include <sstream>

namespace mc {
    std::vector<uint32_t> splitInts(const std::string& s) {
        std::stringstream iss(s);
        uint32_t number;
        std::vector<uint32_t> nums;

        while (iss >> number)
            nums.push_back(number);

        return nums;
    }

    SplitterPanel::SplitterPanel() {
        _createVisuals();
        _setupProperties();
    }

    Size SplitterPanel::_measureSize() {
        Size contentSize = Size(0, 0);

        for (auto& child : getChildren()) {
            auto childSizeWithMargins = child->getDesiredSizeWithMargins();

            if (orientation == Horizontal) {
                contentSize.width += childSizeWithMargins.width;
            } else if (orientation == Vertical) {
                contentSize.height += childSizeWithMargins.height;
            }
        }

        return contentSize;
    }

    void SplitterPanel::_onArrangeChildren() {
        auto totalSize = getComputedSize();
        auto childrenCount = getChildren().size();

        // If there are no children, no need to do the setup
        // steps that involve creating/modifying divider visuals.
        if (childrenCount == 0) {
            return;
        }

        // First make sure that there are enough
        // dividers and reset their dimensions.
        _createNecessaryDividerVisuals();
        _resetDividers();

        std::vector<uint32_t> weights;
        uint32_t weightTotal = 0;

        if (sectionWeights.get().empty()) {
            for (const auto& child : getChildren()) {
                weights.push_back(1);
                weightTotal += 1;
            }
        } else {
            weights = splitInts(sectionWeights);
            for (auto& w : weights) {
                weightTotal += w;
            }
        }

        CORE_ASSERT(weights.size() >= childrenCount,
            "Invalid number of section weights specified for 'sectionWeights', weights: " +
            std::to_string(weights.size()) + " children: " + std::to_string(childrenCount));

        Position nextChildPos = { 0, 0 };

        for (size_t i = 0; i < childrenCount; ++i) {
            auto child = getChild(i);

            if (orientation == Horizontal) {
                double multiplier = static_cast<double>(weights.at(i)) / weightTotal;
                auto sectionWidth = static_cast<uint32_t>(totalSize.width * multiplier);
                auto sectionHeight = totalSize.height;

                // If the child has minimum size constraints, they need to be respected
                if (sectionWidth < child->minWidth) {
                    auto sizeDiff = child->minWidth - sectionWidth;

                    // If the child is the last one, the previous
                    // one needs to be resized to be smaller.
                    if (i == childrenCount - 1 && childrenCount > 1) {
                        auto previousChild = getChild(i - 1);
                        auto previousDivider = d_dividers.at(i - 1);

                        auto previousSize = previousChild->getComputedSize();
                        previousSize.width -= sizeDiff;
                        previousDivider->position->x -= sizeDiff;

                        previousChild->setComputedSize(previousSize);
                        nextChildPos.x -= sizeDiff;
                    } else {
                        totalSize.width -= sizeDiff;
                    }

                    sectionWidth = child->minWidth;
                }

                // If the child has maximum size constraints, they need to be respected
                if (sectionWidth > child->maxWidth) {
                    auto sizeDiff = sectionWidth - child->maxWidth;

                    // If the child is the last one, the previous
                    // one needs to be resized to be larger.
                    if (i == childrenCount - 1 && childrenCount > 1) {
                        auto previousChild = getChild(i - 1);
                        auto previousDivider = d_dividers.at(i - 1);

                        auto previousSize = previousChild->getComputedSize();
                        previousSize.width += sizeDiff;
                        previousDivider->position->x += sizeDiff;

                        previousChild->setComputedSize(previousSize);
                        nextChildPos.x += sizeDiff;
                    } else {
                        totalSize.width -= child->maxWidth;
                        --weightTotal;
                    }

                    sectionWidth = child->maxWidth;
                }

                child->position = nextChildPos;
                child->setComputedSize(Size(sectionWidth, sectionHeight));

                nextChildPos.x += sectionWidth;

                // Place a divider if necessary
                if (i < childrenCount - 1) {
                    auto& divider = d_dividers.at(i);

                    divider->position->x = nextChildPos.x - (dividerSize / 2);
                    divider->position->y = 0;
                    divider->customWidth = dividerSize;
                    divider->customHeight = sectionHeight;
                }

            } else if (orientation == Vertical) {
                double multiplier = static_cast<double>(weights.at(i)) / weightTotal;
                auto sectionHeight = static_cast<uint32_t>(totalSize.height * multiplier);
                auto sectionWidth = totalSize.width;

                // If the child has minimum size constraints, they need to be respected
                if (sectionHeight < child->minHeight) {
                    auto sizeDiff = child->minHeight - sectionHeight;

                    // If the child is the last one, the previous
                    // one needs to be resized to be smaller.
                    if (i == childrenCount - 1 && childrenCount > 1) {
                        auto previousChild = getChild(i - 1);
                        auto previousDivider = d_dividers.at(i - 1);

                        auto previousSize = previousChild->getComputedSize();
                        previousSize.height -= sizeDiff;
                        previousDivider->position->y -= sizeDiff;

                        previousChild->setComputedSize(previousSize);
                        nextChildPos.y -= sizeDiff;
                    } else {
                        totalSize.height -= sizeDiff;
                    }

                    sectionHeight = child->minHeight;
                }

                // If the child has maximum size constraints, they need to be respected
                if (sectionHeight > child->maxHeight) {
                    auto sizeDiff = sectionHeight - child->maxHeight;

                    // If the child is the last one, the previous
                    // one needs to be resized to be larger.
                    if (i == childrenCount - 1 && childrenCount > 1) {
                        auto previousChild = getChild(i - 1);
                        auto previousDivider = d_dividers.at(i - 1);

                        auto previousSize = previousChild->getComputedSize();
                        previousSize.height += sizeDiff;
                        previousDivider->position->y += sizeDiff;

                        previousChild->setComputedSize(previousSize);
                        nextChildPos.y += sizeDiff;
                    } else {
                        totalSize.height -= child->maxHeight;
                        --weightTotal;
                    }

                    sectionHeight = child->maxHeight;
                }

                child->position = nextChildPos;
                child->setComputedSize(Size(sectionWidth, sectionHeight));

                nextChildPos.y += sectionHeight;

                // Place a divider if necessary
                if (i < childrenCount - 1) {
                    auto& divider = d_dividers.at(i);

                    divider->position->x = 0;
                    divider->position->y = nextChildPos.y - (dividerSize / 2);
                    divider->customWidth = sectionWidth;
                    divider->customHeight = dividerSize;
                }
            }
        }
    }

    void SplitterPanel::_createVisuals() {
        // Setup the panel's body rectangle
        auto bodyRect = MakeRef<RectVisual>();

        backgroundColor.forwardAssignment(&bodyRect->color);
        addCoreVisualElement(bodyRect);
    }

    void SplitterPanel::_setupProperties() {
        backgroundColor = Color::gray; // default value
        handleWidgetVisiblePropertyChange(backgroundColor);

        dividerColor = Color(20, 20, 20); // default value
        handleWidgetVisiblePropertyChange(dividerColor);

        movableDividers = false; // default value
        handleWidgetVisiblePropertyChange(movableDividers);

        dividerSize = 2; // default value
        handleWidgetVisiblePropertyChange(dividerSize);

        orientation = Horizontal; // default value
        handleWidgetVisiblePropertyChange(orientation);
        orientation.on("propertyChanged", [this](auto e) {
            signalLayoutChanged();
        });

        sectionWeights = "";
        handleWidgetLayoutPropertyChange(sectionWeights);
        handleWidgetVisiblePropertyChange(sectionWeights);
    }

    void SplitterPanel::_createNecessaryDividerVisuals() {
        size_t childrenCount = getChildren().size();
        size_t dividerCount = d_dividers.size();

        if (dividerCount < (childrenCount - 1)) {
            size_t diff = (childrenCount - 1) - dividerCount;

            for (size_t i = 0; i < diff; ++i) {
                // The trick is to make rectangles
                // thin enough to look like lines.
                auto divider = MakeRef<RectVisual>();
                divider->color = dividerColor;
                dividerColor.forwardAssignment(&divider->color);
                addOverlayVisualElement(divider);

                d_dividers.push_back(divider);
            }
        }
    }

    void SplitterPanel::_resetDividers() {
        for (auto& divider : d_dividers) {
            divider->customWidth = 0;
            divider->customHeight = 0;
            divider->color = dividerColor;
        }
    }
} // namespace mc
