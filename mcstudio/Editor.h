#pragma once
#include <widgets/AllWidgets.h>

namespace mc::mcstudio {
class Editor {
public:
    Editor();

    void ToolboxWidget_OnClick(Shared<Event> e);

private:
    Shared<DockPanel> d_widgetCanvasPanel;
};
} // namespace mc::mcstudio
