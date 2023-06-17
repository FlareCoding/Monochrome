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
    Shared<mcx::McxNode> d_limboNode;
};
} // namespace mc::mcstudio
