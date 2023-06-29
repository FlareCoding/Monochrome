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

    /// @brief Reads the properties of a given widget instance and
    /// assigns them as attributes to a provided mcx node instance.
    /// @param widget Widget instance from which to read the properties
    /// @param mcxNode Mcx node into which to read the properties as attributes
    virtual void extractProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) = 0;

    /// @brief Parses the mcx node and applies the provided
    /// mcx attributes to the appropriate widget instance members.
    /// @param widget Widget instance onto which the parsed attributes should be applied
    /// @param mcxNode Mcx node from which to read the properties and apply them to the widget
    virtual void applyProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) = 0;

    /// @brief Returns a list of available properties for the current widget class
    virtual std::vector<std::string> getAvailableProperties() = 0;

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

    std::string _toString(uint32_t value);
    std::string _toString(int32_t value);
    std::string _toString(bool value);
    std::string _toString(const Color& value);
    std::string _toString(HorizontalAlignment value);
    std::string _toString(VerticalAlignment value);
    std::string _toString(Orientation value);
    std::string _toString(DockAnchor value);
    std::string _toString(OverflowDirection value);
};
} // namespace mc::mcx
