#pragma once
#include <widgets/BaseWidget.h>
#include <mcx/McxNode.h>

namespace mc::mcx {
/// @brief This class is used as a superclass for specific widget adapters
/// that allow parsing the mcx format and producing native widget objects.
class McxParsingAdapter {
public:
    McxParsingAdapter() = default;
    virtual ~McxParsingAdapter() = default;

    /// @brief Creates and returns a native widget instance.
    virtual Shared<BaseWidget> createWidgetInstance(Shared<McxNode>& mcxNode) = 0;

    /// @brief Parses the mcx node and applies the provided
    /// mcx attributes to the appropriate widget instance members.
    /// @param widget Widget instance onto which the parsed attributes should be applied
    /// @param mcxNode Mcx node from which to read the properties and apply them to the widget
    virtual void applyProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) = 0;

protected:
    /// @brief Checks if there is an attribute specifying an event handler name
    /// and if so, attemps to retrieve it and add it to the widget instance.
    /// @param eventName Name of the event to check if there is a handler for
    /// @param mcxNode Reference to the node from which to extract attributes
    /// @param widget Instance of a widget to apply
    void _checkAndApplyEventHandler(
        const std::string& eventName,
        Shared<McxNode>& mcxNode,
        Shared<BaseWidget>& widget
    );
};
} // namespace mc::mcx
