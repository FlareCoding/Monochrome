#include "ProgressBarMcxAdapter.h"
#include <widgets/ProgressBar.h>

namespace mc::mcx {
    Shared<BaseWidget> ProgressBarMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return MakeRef<ProgressBar>();
    }

    void ProgressBarMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        auto progressBar = std::static_pointer_cast<ProgressBar>(widget);
        progressBar->backgroundColor =
            mcxNode->getColorAttribute("backgroundColor", progressBar->backgroundColor);

        progressBar->progressColor =
            mcxNode->getColorAttribute("progressColor", progressBar->progressColor);

        progressBar->value =
            mcxNode->getIntAttribute("value", progressBar->value);

        progressBar->minValue =
            mcxNode->getIntAttribute("minValue", progressBar->minValue);

        progressBar->maxValue =
            mcxNode->getIntAttribute("maxValue", progressBar->maxValue);
    }

    std::vector<std::string> ProgressBarMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "backgroundColor", "progressColor", "value",
            "minValue", "maxValue"
        };

        return props;
    }
} //namespace mc::mcx
