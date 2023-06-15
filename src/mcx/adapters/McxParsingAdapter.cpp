#include "McxParsingAdapter.h"

namespace mc::mcx {
    void McxParsingAdapter::_checkAndApplyEventHandler(
        const std::string& eventName,
        Shared<McxNode>& mcxNode,
        Shared<BaseWidget>& widget
    ) {
        auto handlerName = mcxNode->getAttribute(eventName);
        if (!handlerName.empty()) {
            auto callbackFn = getRegisteredNamedEventHandler(handlerName);
            if (callbackFn) {
                widget->on(eventName, callbackFn);
            } else {
                printf("Invalid event handler name '%s'\n", handlerName.c_str());
            }
        }
    }
} // namespace mc::mcx
