#pragma once
#include <widgets/AllWidgets.h>
#include <mcx/adapters/BaseWidgetMcxAdapter.h>

namespace mc::mcstudio {
class BaseWidgetPropertiesPanel : public StackPanel {
public:
    explicit BaseWidgetPropertiesPanel(Shared<mcx::BaseWidgetMcxAdapter> adapter);

    void update(
        Shared<mcx::McxNode> widgetNode,
        Shared<mcx::McxParsingAdapter> widgetAdapter
    );

private:
    Shared<mcx::BaseWidgetMcxAdapter> d_basePropAdapter;

    void _onWidgetPropertyModified(Shared<Event> e);
};
} // namespace mc::mcstudio
