#include "StackPanelMcxAdapter.h"
#include <widgets/StackPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> StackPanelMcxAdapter::createWidgetInstance() {
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
} //namespace mc::mcx
