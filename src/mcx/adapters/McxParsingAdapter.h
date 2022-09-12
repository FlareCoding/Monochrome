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
    virtual Shared<BaseWidget> createWidgetInstance() = 0;

    /// @brief Parses the mcx node and applies the provided
    /// mcx attributes to the appropriate widget instance members.
    /// @param widget Widget instance onto which the parsed attributes should be applied
    /// @param mcxNode Mcx node from which to read the properties and apply them to the widget
    virtual void applyProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) = 0;
};
} // namespace mc::mcx
