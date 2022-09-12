#pragma once
#include "McxParsingAdapter.h"

namespace mc::mcx {
class StackPanelMcxAdapter : public McxParsingAdapter {
public:
    /// @brief Creates and returns a native widget instance.
    Shared<BaseWidget> createWidgetInstance(Shared<McxNode>& mcxNode) override;

    /// @brief Parses the mcx node and applies the provided
    /// mcx attributes to the appropriate widget instance members.
    /// @param widget Widget instance onto which the parsed attributes should be applied
    /// @param mcxNode Mcx node from which to read the properties and apply them to the widget
    void applyProperties(Shared<BaseWidget>& widget, Shared<McxNode>& mcxNode) override;
};
} //namespace mc::mcx
