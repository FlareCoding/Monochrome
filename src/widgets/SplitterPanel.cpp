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
            }
            else if (orientation == Vertical) {
                contentSize.height += childSizeWithMargins.height;
            }
        }

        return contentSize;
    }

    void SplitterPanel::_onArrangeChildren() {
        // First make sure that there are enough
        // dividers and reset their dimensions.
        _createNecessaryDividerVisuals();
        _resetDividers();

        Position childSlotPosition = Position(0, 0);
        auto totalSize = getComputedSize();
        auto childrenCount = getChildren().size() - d_btns.size();

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

        /*CORE_ASSERT(weights.size() >= childrenCount,
            "Invalid number of section weights specified for 'sectionWeights', weights: " +
            std::to_string(weights.size()) + " children: " + std::to_string(childrenCount));*/

        Position nextChildPos = { 0, 0 };

        for (size_t i = 0; i < childrenCount; ++i) {
            auto child = getChild(i);
            
            if (orientation == Horizontal) {
                double multiplier = static_cast<double>(weights.at(i)) / weightTotal;
                auto sectionWidth = static_cast<uint32_t>(totalSize.width * multiplier);
                auto sectionHeight = totalSize.height;

                child->position = nextChildPos;
                child->setComputedSize(Size(sectionWidth, sectionHeight));

                nextChildPos.x += sectionWidth;

                // Place a divider if necessary
                if (i < childrenCount - 1) {
                    /*auto& divider = d_dividers.at(i);

                    divider->position->x = nextChildPos.x - (dividerSize / 2);
                    divider->position->y = 0;
                    divider->customWidth = dividerSize;
                    divider->customHeight = sectionHeight;*/
                    auto& divider = d_btns.at(i);

                    divider->position->x = nextChildPos.x - (dividerSize / 2);
                    divider->position->y = 0;
                   /* divider->fixedWidth = dividerSize;
                    divider->fixedHeight = sectionHeight;*/
                    divider->setComputedSize(Size(dividerSize, sectionHeight));
                }

            } else if (orientation == Vertical) {
                double multiplier = static_cast<double>(weights.at(i)) / weightTotal;
                auto sectionHeight = static_cast<uint32_t>(totalSize.height * multiplier);
                auto sectionWidth = totalSize.width;

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
        backgroundColor.forwardEmittedEvents(this);

        dividerColor = Color(20, 20, 20); // default value
        dividerColor.forwardEmittedEvents(this);

        movableDividers = false; // default value
        movableDividers.forwardEmittedEvents(this);

        dividerSize = 6; // default value
        dividerSize.forwardEmittedEvents(this);

        orientation = Horizontal; // default value
        orientation.forwardEmittedEvents(this);

        sectionWeights = "";
        sectionWeights.forwardEmittedEvents(this);

        for (int i = 0; i < 2; i++) {
            auto divider = MakeRef<Button>();
            divider->label->text = "";
            divider->backgroundColor = Color::red;
            divider->zIndex = std::numeric_limits<uint32_t>::max();
            divider->borderColor = Color::transparent;
            divider->cursorType = CursorType::SizeWE;
            divider->on("mouseDown", [](auto e) { printf("now\n"); });
            //dividerColor.forwardAssignment(&divider->backgroundColor);

            d_btns.push_back(divider);
            addChild(divider);
        }
    }
    
    void SplitterPanel::_createNecessaryDividerVisuals() {
        //size_t childrenCount = getChildren().size();
        //size_t dividerCount = d_dividers.size();

        //if (dividerCount < (childrenCount - 1)) {
        //    size_t diff = (childrenCount - 1) - dividerCount;
        //    
        //    for (size_t i = 0; i < diff; ++i) {
        //        // The trick is to make rectangles
        //        // thin enough to look like lines.
        //        auto divider = MakeRef<RectVisual>();
        //        divider->color = dividerColor;
        //        dividerColor.forwardAssignment(&divider->color);
        //        addOverlayVisualElement(divider);

        //        d_dividers.push_back(divider);
        //    }
        //}
    }

    void SplitterPanel::_resetDividers() {
       /* for (auto& divider : d_dividers) {
            divider->customWidth = 0;
            divider->customHeight = 0;
            divider->color = dividerColor;
        }*/
    }
} // namespace mc
