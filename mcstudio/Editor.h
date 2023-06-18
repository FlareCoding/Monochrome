#pragma once
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
#include <mcx/adapters/BaseWidgetMcxAdapter.h>

namespace mc::mcstudio {
class Editor {
public:
    Editor();

    void ToolboxWidget_OnClick(Shared<Event> e);
    void RootContainerSelection_OnClick(Shared<Event> e);

private:
    // Creates and returns new widget from a given widget name
    Shared<BaseWidget> _spawnWidget(const std::string& widgetName);

    void _setSelectedWidget(Shared<BaseWidget> widget);

    // Node responsible for holding current selected widget's mcx information
    Shared<mcx::McxNode> d_selectedWidgetNode;

    // Adapter for basic widget properties
    Shared<mcx::BaseWidgetMcxAdapter> d_baseWidgetAdapter;

    // App's root container widget
    Shared<BaseContainerWidget> d_appRootContainer;

    // Currently selected widget (either newly added or clicked on with a mouse)
    Shared<BaseWidget> d_selectedWidget;
};
} // namespace mc::mcstudio
