#include "SplitterPanelMcxAdapter.h"
#include <widgets/SplitterPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> SplitterPanelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<SplitterPanel>();
    }

    void SplitterPanelMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto splitterPanel = std::static_pointer_cast<SplitterPanel>(widget);
        splitterPanel->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", splitterPanel->backgroundColor);

        splitterPanel->dividerColor =
            mcxNode->getColorAttribute("dividerColor", splitterPanel->dividerColor);

        splitterPanel->movableDividers =
            mcxNode->getBoolAttribute("movableDividers");

        splitterPanel->dividerSize =
            mcxNode->getUIntAttribute("dividerSize", splitterPanel->dividerSize);

        auto orientation = mcxNode->getAttribute("orientation", "horizontal");
        if (orientation == "horizontal") {
            splitterPanel->orientation = Horizontal;
        } else {
            splitterPanel->orientation = Vertical;
        }

        splitterPanel->sectionWeights =
            mcxNode->getAttribute("sectionWeights");
    }
} //namespace mc::mcx
