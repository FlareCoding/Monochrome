#include "ScrollPanelMcxAdapter.h"
#include <widgets/ScrollPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> ScrollPanelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<ScrollPanel>();
    }

    void ScrollPanelMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto scrollPanel = std::static_pointer_cast<ScrollPanel>(widget);
        scrollPanel->cornerRadius =
            mcxNode->getUIntAttribute("cornerRadius", scrollPanel->cornerRadius);

        scrollPanel->scrollbarTracksColor =
            mcxNode->getColorAttribute("trackColor", scrollPanel->scrollbarTracksColor);

        scrollPanel->scrollbarTrackButtonBackground = mcxNode->getColorAttribute(
                "trackButtonBackground",
                scrollPanel->scrollbarTrackButtonBackground
        );

        scrollPanel->scrollbarTrackButtonColor =
            mcxNode->getColorAttribute("trackButtonColor", scrollPanel->scrollbarTrackButtonColor);

        scrollPanel->scrollbarColor =
            mcxNode->getColorAttribute("scrollbarColor", scrollPanel->scrollbarColor);
    }
} //namespace mc::mcx
