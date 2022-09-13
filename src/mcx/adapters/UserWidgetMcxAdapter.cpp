#include "UserWidgetMcxAdapter.h"
#include <mcx/McxEngine.h>

namespace mc::mcx {
    Shared<BaseWidget> UserWidgetMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        auto userWidgetFilepath = mcxNode->getAttribute("path");
        return McxEngine::parseUserWidgetFile(userWidgetFilepath);
    }

    void UserWidgetMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {}
} //namespace mc::mcx