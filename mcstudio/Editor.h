#pragma once
#include <widgets/AllWidgets.h>
#include "mcx/McxEngine.h"

namespace mc::mcstudio {
class Editor {
public:
    Editor();

    void ToolboxWidget_OnClick(Shared<Event> e);
    void RootContainerSelection_OnClick(Shared<Event> e);

private:
    void _setSelectedWidget(Shared<BaseWidget> widget);

    Shared<mcx::McxNode> d_limboNode;
    Shared<BaseContainerWidget> d_appRootContainer;
    Shared<BaseWidget> d_selectedWidget;
};
} // namespace mc::mcstudio
