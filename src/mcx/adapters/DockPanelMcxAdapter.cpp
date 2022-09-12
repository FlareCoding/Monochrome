#include "DockPanelMcxAdapter.h"
#include <widgets/DockPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> DockPanelMcxAdapter::createWidgetInstance() {
        return MakeRef<DockPanel>();
    }

    void DockPanelMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto dockPanel = std::static_pointer_cast<DockPanel>(widget);
        dockPanel->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", dockPanel->backgroundColor);

        dockPanel->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", dockPanel->cornerRadius);
    }
} //namespace mc::mcx
