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

        scrollPanel->scrollSensitivity =
            mcxNode->getIntAttribute("scrollSensitivity", scrollPanel->scrollSensitivity);

        scrollPanel->autoscroll = mcxNode->getBoolAttribute("autoscroll");
    }

    std::vector<std::string> ScrollPanelMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "cornerRadius", "trackColor", "trackButtonBackground",
            "trackButtonColor", "scrollbarColor", "scrollSensitivity",
            "autoscroll"
        };

        return props;
    }
} //namespace mc::mcx
