#pragma once
#include "McxParsingAdapter.h"

namespace mc::mcx {
class ScrollPanelMcxAdapter : public McxParsingAdapter {
public:
    /// @brief Creates and returns a native widget instance.
    Shared<BaseWidget> createWidgetInstance(Shared<McxNode>& mcxNode) override;

    /// @brief Reads the properties of a given widget instance and
    /// assigns them as attributes to a provided mcx node instance.
    /// @param widget Widget instance from which to read the properties
    /// @param mcxNode Mcx node into which to read the properties as attributes
    void extractProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) override;

    /// @brief Parses the mcx node and applies the provided
    /// mcx attributes to the appropriate widget instance members.
    /// @param widget Widget instance onto which the parsed attributes should be applied
    /// @param mcxNode Mcx node from which to read the properties and apply them to the widget
    void applyProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) override;

    /// @brief Returns a list of available properties for the current widget class
    std::vector<std::string> getAvailableProperties() override;
};
} //namespace mc::mcx
