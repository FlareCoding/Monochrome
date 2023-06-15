#include "Editor.h"

namespace mc::mcstudio {
    Editor::Editor() {
        registerNamedEventHandler("ToolboxWidget_OnClick", &Editor::ToolboxWidget_OnClick, this);
    }

    void Editor::ToolboxWidget_OnClick(Shared<Event> e) {
        if (!d_widgetCanvasPanel) {
            d_widgetCanvasPanel = getWidgetById<DockPanel>("widgetCanvasPanel");
            CORE_ASSERT(d_widgetCanvasPanel, "Failed to get a reference to widget canvas panel");
        }

        auto target = static_cast<Button*>(e->target);
        printf("adding widget: %s\n", target->label->text.get().c_str());
    }
} // namespace mc::mcstudio
