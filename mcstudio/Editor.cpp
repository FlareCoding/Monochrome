#include "Editor.h"
#include "mcx/McxEngine.h"
#include "mcx/adapters/BaseWidgetMcxAdapter.h"

namespace mc::mcstudio {
    Editor::Editor() {
        registerNamedEventHandler("ToolboxWidget_OnClick", &Editor::ToolboxWidget_OnClick, this);
        registerNamedEventHandler("RootContainerSelection_OnClick",
                &Editor::RootContainerSelection_OnClick, this);
    }

    void Editor::ToolboxWidget_OnClick(Shared<Event> e) {
        auto rootContainer = getWidgetById<BaseContainerWidget>("appRootContainer");
        if (!rootContainer) {
            return;
        }

        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);

        Shared<mcx::McxNode> limboNode = nullptr;
        auto widgetInstance = mcxAdapter->createWidgetInstance(limboNode);

        rootContainer->addChild(widgetInstance);

        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();

        auto baseAdapter = MakeRef<mcx::BaseWidgetMcxAdapter>();

        for (auto& prop : baseAdapter->getAvailableProperties()) {
            auto lbl = MakeRef<Label>();
            lbl->text = prop;
            propertiesListPanel->addChild(lbl);
        }

        auto lbl = MakeRef<Label>();
        lbl->text = " ";
        propertiesListPanel->addChild(lbl);

        for (auto& prop : mcxAdapter->getAvailableProperties()) {
            auto lbl = MakeRef<Label>();
            lbl->text = prop;
            propertiesListPanel->addChild(lbl);
        }
    }

    void Editor::RootContainerSelection_OnClick(Shared<Event> e) {
        getWidgetById("initialRootContainerPromptLabel")->hide();
        getWidgetById("initialRootContainerPromptPanel")->hide();

        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);

        Shared<mcx::McxNode> limboNode = nullptr;
        auto widgetInstance = mcxAdapter->createWidgetInstance(limboNode);
        widgetInstance->fixedWidth = 800;
        widgetInstance->fixedHeight = 800;
        widgetInstance->marginLeft = 20;

        auto editorPanel = getWidgetById<StackPanel>("mcEditorPanel");
        editorPanel->addChild(widgetInstance);

        registerWidgetWithUserId("appRootContainer", widgetInstance);
    }
} // namespace mc::mcstudio
