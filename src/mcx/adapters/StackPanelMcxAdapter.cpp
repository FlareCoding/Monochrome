#include "StackPanelMcxAdapter.h"
#include <widgets/StackPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> StackPanelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<StackPanel>();
    }

    void StackPanelMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto stackPanel = std::static_pointer_cast<StackPanel>(widget);
        stackPanel->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", stackPanel->backgroundColor);

        stackPanel->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", stackPanel->cornerRadius);

        auto orientation = mcxNode->getAttribute("orientation", "vertical");
        if (orientation == "horizontal") {
            stackPanel->orientation = Horizontal;
        } else {
            stackPanel->orientation = Vertical;
        }
    }

    std::vector<std::string> StackPanelMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "backgroundColor", "cornerRadius", "orientation"
        };

        return props;
    }
} //namespace mc::mcx
