#include "McxEngine.h"
#include "McxNode.h"
#include <filesystem>
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"

// Widget adapters
#include "adapters/BaseWidgetMcxAdapter.h"
#include "adapters/StackPanelMcxAdapter.h"
#include "adapters/DockPanelMcxAdapter.h"
#include "adapters/ScrollPanelMcxAdapter.h"
#include "adapters/LabelMcxAdapter.h"
#include "adapters/ButtonMcxAdapter.h"
#include "adapters/CheckboxMcxAdapter.h"
#include "adapters/ComboboxMcxAdapter.h"
#include "adapters/DropdownButtonMcxAdapter.h"
#include "adapters/MenuListMcxAdapter.h"
#include "adapters/EntryMcxAdapter.h"
#include "adapters/ProgressBarMcxAdapter.h"
#include "adapters/TreeViewGroupMcxAdapter.h"
#include "adapters/TreeViewMcxAdapter.h"

namespace mc::mcx {
    bool McxEngine::s_mcxEngineInitialized = false;
    std::map<std::string, Shared<McxParsingAdapter>> McxEngine::s_mcxAdapters;

    static Shared<BaseWidgetMcxAdapter> s_baseWidgetMcxAdapter = MakeRef<BaseWidgetMcxAdapter>();

    Shared<ClassicWindow> McxEngine::parseWindowFile(const std::string& path) {
        if (!s_mcxEngineInitialized) {
            _initializeMcxEngine();
        }

        Shared<ClassicWindow> window = nullptr;

        bool fileExists = std::filesystem::is_regular_file(path);
        if (!fileExists) {
            printf("'%s' could not be found\n", path.c_str());
            return window;
        }

        rapidxml::file<> xmlFile(path.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(xmlFile.data());

        auto rootNode = MakeRef<McxNode>(doc.first_node());
        const auto rootType = rootNode->getAttribute("type");

        // Making sure that the type of the root node is a window
        if (rootType != "window") {
            printf("McxEngineError: root node is not a window, '%s' type found\n",
                rootType.c_str());
            return window;
        }

        auto width = rootNode->getUIntAttribute("width");
        auto height = rootNode->getUIntAttribute("height");
        auto title = rootNode->getAttribute("title", "Monochrome App");

        window = MakeRef<ClassicWindow>(width, height, title);

        auto& children = rootNode->getChildren();
        if (children.size() != 1) {
            printf("Window must have only one root widget\n");
            return window;
        }

        auto& rootWidgetNode = children.at(0);

        auto rootWidgetInstance = parseWidget(rootWidgetNode);

        if (!rootWidgetInstance->isContainer()) {
            printf("Root widget instance must be a BaseContainerWidget\n");
            return window;
        }

        window->setRootWidget(std::static_pointer_cast<BaseContainerWidget>(rootWidgetInstance));
        return window;
    }

    Shared<ClassicWindow> McxEngine::parseWindowSource(const std::string& source) {
        if (!s_mcxEngineInitialized) {
            _initializeMcxEngine();
        }

        return nullptr;
    }

    Shared<BaseWidget> McxEngine::parseWidget(Shared<McxNode>& node) {
        // Get the appropriate parsing adapter
        const auto mcxAdapter = s_mcxAdapters.at(node->getType());

        // Create the native widget instance
        auto widgetInstance = mcxAdapter->createWidgetInstance();

        // Apply basic widget properties
        s_baseWidgetMcxAdapter->applyProperties(widgetInstance, node);

        // Apply widget-specific properties
        mcxAdapter->applyProperties(widgetInstance, node);

        // Check if child nodes need to be processed
        if (node->childrenHandled) {
            return widgetInstance;
        }

        auto& childNodes = node->getChildren();
        if (childNodes.size()) {
            CORE_ASSERT(widgetInstance->isContainer(),
                "Cannot parse children for a node that isn't a container widget");
        }

        for (auto& child : childNodes) {
            auto childWidgetInstance = parseWidget(child);

            // Add the parsed child widget instance to this instance
            auto container = std::static_pointer_cast<BaseContainerWidget>(widgetInstance);
            container->addChild(childWidgetInstance);
        }

        return widgetInstance;
    }

    void McxEngine::registerMcxAdapter(
        const std::string& name,
        Shared<McxParsingAdapter> adapter
    ) {
       s_mcxAdapters[name] = adapter;
    }

    Shared<McxParsingAdapter> McxEngine::getMcxAdapter(const std::string& widgetType) {
        if (s_mcxAdapters.find(widgetType) == s_mcxAdapters.end()) {
            return nullptr;
        }

        return s_mcxAdapters.at(widgetType);
    }

    void McxEngine::_initializeMcxEngine() {
        registerMcxAdapter("StackPanel", MakeRef<StackPanelMcxAdapter>());
        registerMcxAdapter("DockPanel", MakeRef<DockPanelMcxAdapter>());
        registerMcxAdapter("ScrollPanel", MakeRef<ScrollPanelMcxAdapter>());
        registerMcxAdapter("Label", MakeRef<LabelMcxAdapter>());
        registerMcxAdapter("Button", MakeRef<ButtonMcxAdapter>());
        registerMcxAdapter("Checkbox", MakeRef<CheckboxMcxAdapter>());
        registerMcxAdapter("Combobox", MakeRef<ComboboxMcxAdapter>());
        registerMcxAdapter("DropdownButton", MakeRef<DropdownButtonMcxAdapter>());
        registerMcxAdapter("MenuList", MakeRef<MenuListMcxAdapter>());
        registerMcxAdapter("Entry", MakeRef<EntryMcxAdapter>());
        registerMcxAdapter("ProgressBar", MakeRef<ProgressBarMcxAdapter>());
        registerMcxAdapter("TreeViewGroup", MakeRef<TreeViewGroupMcxAdapter>());
        registerMcxAdapter("TreeView", MakeRef<TreeViewMcxAdapter>());

        s_mcxEngineInitialized = true;
    }
} // namespace mc::mcx
