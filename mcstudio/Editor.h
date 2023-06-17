#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class Editor {
public:
    Editor();

    void ToolboxWidget_OnClick(Shared<Event> e);
    void RootContainerSelection_OnClick(Shared<Event> e);
};
} // namespace mc::mcstudio
