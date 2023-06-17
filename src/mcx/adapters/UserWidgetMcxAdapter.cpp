#include "UserWidgetMcxAdapter.h"
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> UserWidgetMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        auto userWidgetFilepath = mcxNode->getAttribute("path");
        return McxEngine::parseUserWidgetFile(userWidgetFilepath);
    }

    void UserWidgetMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {}

    void UserWidgetMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {}
    
    std::vector<std::string> UserWidgetMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {};
        return props;
    }
} //namespace mc::mcx
