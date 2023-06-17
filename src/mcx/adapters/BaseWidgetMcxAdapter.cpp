#include "BaseWidgetMcxAdapter.h"
#include <widgets/StackPanel.h>

namespace mc::mcx {
    Shared<BaseWidget> BaseWidgetMcxAdapter::createWidgetInstance(Shared<McxNode>& mcxNode) {
        return nullptr;
    }

    void BaseWidgetMcxAdapter::extractProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        mcxNode->setAttribute("zIndex", _toString(widget->zIndex));
        mcxNode->setAttribute("visible", _toString(widget->visible));
        mcxNode->setAttribute("fixedWidth", _toString(widget->fixedWidth));
        mcxNode->setAttribute("fixedHeight", _toString(widget->fixedHeight));
        mcxNode->setAttribute("minWidth", _toString(widget->minWidth));
        mcxNode->setAttribute("minHeight", _toString(widget->minHeight));
        mcxNode->setAttribute("maxWidth", _toString(widget->maxWidth));
        mcxNode->setAttribute("maxHeight", _toString(widget->maxHeight));
        mcxNode->setAttribute("marginTop", _toString(widget->marginTop));
        mcxNode->setAttribute("marginBottom", _toString(widget->marginBottom));
        mcxNode->setAttribute("marginLeft", _toString(widget->marginLeft));
        mcxNode->setAttribute("marginRight", _toString(widget->marginRight));
        mcxNode->setAttribute("horizontalAlignment", _toString(widget->horizontalAlignment));
        mcxNode->setAttribute("verticalAlignment", _toString(widget->verticalAlignment));
        mcxNode->setAttribute("dockAnchor", _toString(widget->dockAnchor));
    }

    void BaseWidgetMcxAdapter::applyProperties(
        Shared<BaseWidget>& widget,
        Shared<McxNode>& mcxNode
    ) {
        widget->zIndex =
            mcxNode->getUIntAttribute("zIndex", widget->zIndex);

        widget->visible =
            mcxNode->getBoolAttribute("visible", widget->visible);

        widget->fixedWidth =
            mcxNode->getUIntAttribute("fixedWidth", widget->fixedWidth);

        widget->fixedHeight =
            mcxNode->getUIntAttribute("fixedHeight", widget->fixedHeight);

        widget->minWidth =
            mcxNode->getUIntAttribute("minWidth", widget->minWidth);

        widget->minHeight =
            mcxNode->getUIntAttribute("minHeight", widget->minHeight);

        widget->maxWidth =
            mcxNode->getUIntAttribute("maxWidth", widget->maxWidth);

        widget->maxHeight =
            mcxNode->getUIntAttribute("maxHeight", widget->maxHeight);

        widget->marginTop =
            mcxNode->getUIntAttribute("marginTop", widget->marginTop);

        widget->marginBottom =
            mcxNode->getUIntAttribute("marginBottom", widget->marginBottom);

        widget->marginLeft =
            mcxNode->getUIntAttribute("marginLeft", widget->marginLeft);

        widget->marginRight =
            mcxNode->getUIntAttribute("marginRight", widget->marginRight);

        auto horizontalAlignment = mcxNode->getAttribute("horizontalAlignment", "left");
        if (horizontalAlignment == "left") {
            widget->horizontalAlignment = HALeft;
        } else if (horizontalAlignment == "right") {
            widget->horizontalAlignment = HARight;
        } else if (horizontalAlignment == "center") {
            widget->horizontalAlignment = HACenter;
        } else if (horizontalAlignment == "fill") {
            widget->horizontalAlignment = HAFill;
        }

        auto verticalAlignment = mcxNode->getAttribute("verticalAlignment", "top");
        if (verticalAlignment == "top") {
            widget->verticalAlignment = VATop;
        } else if (verticalAlignment == "bottom") {
            widget->verticalAlignment = VABottom;
        } else if (verticalAlignment == "center") {
            widget->verticalAlignment = VACenter;
        } else if (verticalAlignment == "fill") {
            widget->verticalAlignment = VAFill;
        }

        auto dockAnchor = mcxNode->getAttribute("dockAnchor", "left");
        if (dockAnchor == "left") {
            widget->dockAnchor = DockAnchor::Left;
        } else if (dockAnchor == "right") {
            widget->dockAnchor = DockAnchor::Right;
        } else if (dockAnchor == "top") {
            widget->dockAnchor = DockAnchor::Top;
        } else if (dockAnchor == "bottom") {
            widget->dockAnchor = DockAnchor::Bottom;
        } else if (dockAnchor == "fill") {
            widget->dockAnchor = DockAnchor::Fill;
        }

        // Parsing the user defined ID
        auto userDefinedId = mcxNode->getAttribute("id");
        if (!userDefinedId.empty()) {
            registerWidgetWithUserId(userDefinedId, widget);
        }

        // Parsing event handlers
        for (auto& eventName : widget->inspectAllowedEvents()) {
            _checkAndApplyEventHandler(eventName, mcxNode, widget);
        }
    }

    std::vector<std::string> BaseWidgetMcxAdapter::getAvailableProperties() {
        static const std::vector<std::string> props = {
            "id", "zIndex", "visible", "fixedWidth", "fixedHeight",
            "minWidth", "minHeight", "maxWidth", "maxHeight",
            "marginTop", "marginBottom", "marginLeft", "marginRight",
            "horizontalAlignment", "verticalAlignment", "dockAnchor",
        };

        return props;
    }
} //namespace mc::mcx
