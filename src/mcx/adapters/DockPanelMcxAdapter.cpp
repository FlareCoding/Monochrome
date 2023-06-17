#include "DockPanelMcxAdapter.h"
#include <widgets/DockPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> DockPanelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<DockPanel>();
    }

    void DockPanelMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto dockPanel = std::static_pointer_cast<DockPanel>(widget);

        mcxNode->setAttribute("backgroundColor", _toString(dockPanel->backgroundColor));
        mcxNode->setAttribute("cornerRadius", _toString(dockPanel->cornerRadius));
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

    std::vector<std::string> DockPanelMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "backgroundColor", "cornerRadius"
        };

        return props;
    }
} //namespace mc::mcx
