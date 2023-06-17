#include "ScrollPanelMcxAdapter.h"
#include <widgets/ScrollPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> ScrollPanelMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<ScrollPanel>();
    }

    void ScrollPanelMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto scrollPanel = std::static_pointer_cast<ScrollPanel>(widget);

        mcxNode->setAttribute("cornerRadius", _toString(scrollPanel->cornerRadius));
        mcxNode->setAttribute("trackColor", _toString(scrollPanel->scrollbarTracksColor));
        mcxNode->setAttribute("trackButtonBackground",
            _toString(scrollPanel->scrollbarTrackButtonBackground));
        mcxNode->setAttribute("trackButtonColor",
            _toString(scrollPanel->scrollbarTrackButtonColor));
        mcxNode->setAttribute("scrollbarColor", _toString(scrollPanel->scrollbarColor));
        mcxNode->setAttribute("scrollSensitivity", _toString(scrollPanel->scrollSensitivity));
        mcxNode->setAttribute("autoscroll", _toString(scrollPanel->autoscroll));
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
