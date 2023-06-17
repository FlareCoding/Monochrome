#include "Editor.h"
#include "mcx/adapters/BaseWidgetMcxAdapter.h"

namespace mc::mcstudio {
    Editor::Editor() {
        d_limboNode = MakeRef<mcx::McxNode>();

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

        auto widgetInstance = mcxAdapter->createWidgetInstance(d_limboNode);
        mcxAdapter->extractProperties(widgetInstance, d_limboNode);

        rootContainer->addChild(widgetInstance);

        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();

        auto baseAdapter = MakeRef<mcx::BaseWidgetMcxAdapter>();
        baseAdapter->extractProperties(widgetInstance, d_limboNode);

        for (auto& prop : baseAdapter->getAvailableProperties()) {
            auto container = MakeRef<StackPanel>();
            container->orientation = Horizontal;
            container->backgroundColor = Color::transparent;

            auto lbl = MakeRef<Label>();
            lbl->text = prop;
            lbl->fixedWidth = 110;
            lbl->marginLeft = 10;
            lbl->alignment = "left";
            container->addChild(lbl);

            auto entry = MakeRef<Entry>();
            entry->text = d_limboNode->getAttribute(prop);
            entry->fixedWidth = 150;
            container->addChild(entry);

            propertiesListPanel->addChild(container);
        }

        auto lbl = MakeRef<Label>();
        lbl->text = " ";
        propertiesListPanel->addChild(lbl);

        for (auto& prop : mcxAdapter->getAvailableProperties()) {
            auto container = MakeRef<StackPanel>();
            container->orientation = Horizontal;
            container->backgroundColor = Color::transparent;

            auto lbl = MakeRef<Label>();
            lbl->text = prop;
            lbl->fixedWidth = 110;
            lbl->marginLeft = 10;
            lbl->alignment = "left";
            container->addChild(lbl);

            auto entry = MakeRef<Entry>();
            entry->text = d_limboNode->getAttribute(prop);;
            entry->fixedWidth = 150;
            entry->on("entered", [](Shared<Event> e) {
                auto target = static_cast<Entry*>(e->target);
                printf("%s\n", target->text.get().c_str());
            });
            container->addChild(entry);

            propertiesListPanel->addChild(container);
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
