#include "McxParsingAdapter.h"
#include "BaseWidgetMcxAdapter.h"
#include <mcx/McxEngine.h>

namespace mc::mcx {
    static void __diffPropertiesToExport(Shared<BaseWidget>& widget, Shared<McxNode>& node) {
        static auto s_baseWidgetMcxAdapter = MakeRef<BaseWidgetMcxAdapter>();

        // Find the right adapter for the widget and extract its properties onto the current node
        auto widgetAdapter = McxEngine::getMcxAdapter(widget->getWidgetName());
        widgetAdapter->extractProperties(widget, node);
        s_baseWidgetMcxAdapter->extractProperties(widget, node);

        // Construct a "pure" default widget instance to compare give widget's properties to
        auto pureInstance = widgetAdapter->createWidgetInstance(node);
        auto pureInstanceNode = MakeRef<McxNode>();
        widgetAdapter->extractProperties(pureInstance, pureInstanceNode);
        s_baseWidgetMcxAdapter->extractProperties(pureInstance, pureInstanceNode);

        // Keep track of identical properties that should be removed from the node
        std::vector<std::string> attribsToRemove;

        for (auto& [attribName, attribValue] : node->getAttribs()) {
            auto pureInstanceAttribValue = pureInstanceNode->getAttribute(attribName);

            if (attribValue == pureInstanceAttribValue) {
                attribsToRemove.push_back(attribName);
            }
        }

        // Remove unnecessary identical properties from the
        // mcx node to prevent redundant property exporting.
        for (auto& attrib : attribsToRemove) {
            node->deleteAttribute(attrib);
        }
    }

    Shared<McxNode> McxParsingAdapter::createMcxNodeFromWidget(Shared<BaseWidget> widget) {
        auto node = MakeRef<McxNode>(widget->getWidgetName());

        // Call the virtual callback function
        _onCreateMcxNodeFromWidget(widget, node);

        // Extract only the necessary widget properties and place them onto the node
        __diffPropertiesToExport(widget, node);

        // If a widget is a container and its children
        // need to be processed, create children mcx nodes.
        if (!node->childrenHandled && widget->isContainer()) {
            auto container = std::static_pointer_cast<BaseContainerWidget>(widget);

            // Traverse every child widget and create an mcx node for it
            for (auto& childWidget : container->getChildren()) {
                // Get the mcx adapter for the child widget
                auto& childWidgetAdapter = McxEngine::getMcxAdapter(childWidget->getWidgetName());

                // Create the child mcx node
                auto childNode = childWidgetAdapter->createMcxNodeFromWidget(childWidget);

                // Add the child mcx node to the current children list
                node->getChildren().push_back(childNode);
            }
        }

        return node;
    }

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

    std::string McxParsingAdapter::_toString(uint32_t value) {
        return std::to_string(value);
    }

    std::string McxParsingAdapter::_toString(int32_t value) {
        return std::to_string(value);
    }

    std::string McxParsingAdapter::_toString(bool value) {
        return (value ? "true" : "false");
    }

    std::string McxParsingAdapter::_toString(const Color& value) {
        std::string result =
                "Color(" + _toString(value.r) + ", " + _toString(value.g) +
                ", " + _toString(value.b);

        if (value.a != 255) {
            result += ", " + _toString(value.a);
        }

        result += ")";
        return result;
    }

    std::string McxParsingAdapter::_toString(HorizontalAlignment value) {
        switch (value) {
        case HALeft: return "left";
        case HARight: return "right";
        case HACenter: return "center";
        case HAFill: return "fill";
        default: return "";
        }
    }

    std::string McxParsingAdapter::_toString(VerticalAlignment value) {
        switch (value) {
        case VATop: return "top";
        case VABottom: return "bottom";
        case VACenter: return "center";
        case VAFill: return "fill";
        default: return "";
        }
    }

    std::string McxParsingAdapter::_toString(Orientation value) {
        switch (value) {
        case Horizontal: return "horizontal";
        case Vertical: return "vertical";
        default: return "";
        }
    }

    std::string McxParsingAdapter::_toString(DockAnchor value) {
        switch (value) {
        case DockAnchor::Top: return "top";
        case DockAnchor::Bottom: return "bottom";
        case DockAnchor::Left: return "left";
        case DockAnchor::Right: return "right";
        case DockAnchor::Fill: return "fill";
        default: return "";
        }
    }

    std::string McxParsingAdapter::_toString(OverflowDirection value) {
        switch (value) {
        case OverflowDirection::Up: return "up";
        case OverflowDirection::Down: return "down";
        case OverflowDirection::Left: return "left";
        case OverflowDirection::Right: return "right";
        default: return "";
        }
    }
} // namespace mc::mcx
