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
