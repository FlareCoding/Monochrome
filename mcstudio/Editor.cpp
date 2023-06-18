#include "Editor.h"
#include <mcx/adapters/BaseWidgetMcxAdapter.h>
#include <core/InternalFlags.h>

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
        rootContainer->addChild(widgetInstance);
        
        _setSelectedWidget(widgetInstance);
    }

    void Editor::RootContainerSelection_OnClick(Shared<Event> e) {
        getWidgetById("initialRootContainerPromptLabel")->hide();
        getWidgetById("initialRootContainerPromptPanel")->hide();

        auto target = static_cast<Button*>(e->target);
        auto widgetName = target->label->text.get();
        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(widgetName);

        Shared<mcx::McxNode> limboNode = nullptr;
        d_appRootContainer = std::static_pointer_cast<BaseContainerWidget>(
            mcxAdapter->createWidgetInstance(limboNode));

        d_appRootContainer->fixedWidth = 800;
        d_appRootContainer->fixedHeight = 800;
        d_appRootContainer->marginLeft = 20;

        auto editorPanel = getWidgetById<StackPanel>("mcEditorPanel");
        editorPanel->addChild(d_appRootContainer);

        registerWidgetWithUserId("appRootContainer", d_appRootContainer);

        d_appRootContainer->on("mouseUp", [](Shared<Event> e) {
            e->stopPropagation();
        });

        d_appRootContainer->on("mouseDown", [this](Shared<Event> e) {
            e->stopPropagation();

            if (!d_appRootContainer) {
                return;
            }

            auto& children = d_appRootContainer->getChildren();
            if (children.empty()) {
                return;
            }

            auto mbe = std::static_pointer_cast<MouseButtonEvent>(e);
            auto mousePos = mbe->getLocation();

            for (auto& child : children) {
                auto childPos = child->getPositionInWindow();
                auto frame = Frame(childPos, child->getComputedSizeWithMargins());
                if (frame.containsPoint(mousePos)) {
                    _setSelectedWidget(child);
                    break;
                }
            }
        });
    }
    
    void Editor::_setSelectedWidget(Shared<BaseWidget> widget) {
        if (d_selectedWidget) {
            d_selectedWidget->unfocus();
        }

        d_selectedWidget = widget;
        d_limboNode->removeAllAttributes();

        auto mcxAdapter = mcx::McxEngine::getMcxAdapter(d_selectedWidget->getWidgetName());
        mcxAdapter->extractProperties(d_selectedWidget, d_limboNode);

        auto propertiesListPanel = getWidgetById<StackPanel>("propertiesListPanel");
        propertiesListPanel->removeAllChildren();

        auto baseAdapter = MakeRef<mcx::BaseWidgetMcxAdapter>();
        baseAdapter->extractProperties(d_selectedWidget, d_limboNode);

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
            entry->on("entered", [this, mcxAdapter, baseAdapter](Shared<Event> e) {
                auto target = static_cast<Entry*>(e->target);
                auto parentContainer = static_cast<StackPanel*>(target->getParent());
                auto label = std::static_pointer_cast<Label>(parentContainer->getChild(0));

                auto propName = label->text.get();
                auto propValue = target->text.get();
                d_limboNode->setAttribute(propName, propValue);
                baseAdapter->applyProperties(d_selectedWidget, d_limboNode);
            });
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
            entry->on("entered", [this, mcxAdapter](Shared<Event> e) {
                auto target = static_cast<Entry*>(e->target);
                auto parentContainer = static_cast<StackPanel*>(target->getParent());
                auto label = std::static_pointer_cast<Label>(parentContainer->getChild(0));

                auto propName = label->text.get();
                auto propValue = target->text.get();
                d_limboNode->setAttribute(propName, propValue);
                mcxAdapter->applyProperties(d_selectedWidget, d_limboNode);
            });
            container->addChild(entry);

            propertiesListPanel->addChild(container);
        }
    }
} // namespace mc::mcstudio
