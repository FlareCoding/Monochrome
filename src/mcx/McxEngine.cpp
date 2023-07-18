#include "McxEngine.h"
#include "McxNode.h"
#include <filesystem>
#include <sstream>
#include <iterator>
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml.hpp"

// Widget adapters
#include "adapters/BaseWidgetMcxAdapter.h"
#include "adapters/StackPanelMcxAdapter.h"
#include "adapters/DockPanelMcxAdapter.h"
#include "adapters/ScrollPanelMcxAdapter.h"
#include "adapters/SplitterPanelMcxAdapter.h"
#include "adapters/LabelMcxAdapter.h"
#include "adapters/ButtonMcxAdapter.h"
#include "adapters/CheckboxMcxAdapter.h"
#include "adapters/ComboboxMcxAdapter.h"
#include "adapters/DropdownButtonMcxAdapter.h"
#include "adapters/MenuListMcxAdapter.h"
#include "adapters/EntryMcxAdapter.h"
#include "adapters/ProgressBarMcxAdapter.h"
#include "adapters/TreeViewMcxAdapter.h"
#include "adapters/TabViewMcxAdapter.h"
#include "adapters/ImageMcxAdapter.h"
#include "adapters/UserWidgetMcxAdapter.h"

// Ignore rapidxml-related stack size warning
#pragma warning(disable: 6262)

namespace mc::mcx {
    bool McxEngine::s_mcxEngineInitialized = false;
    std::map<std::string, Shared<McxParsingAdapter>> McxEngine::s_mcxAdapters;
    std::map<std::string, Shared<McxNode>> McxEngine::s_loadedStyles;
    std::string McxEngine::s_mcxRootDirectory = "";

    static Shared<BaseWidgetMcxAdapter> s_baseWidgetMcxAdapter = MakeRef<BaseWidgetMcxAdapter>();

    void McxEngine::setRootMcxDirectory(const std::string& path) {
        CORE_ASSERT(path.empty() || std::filesystem::is_directory(path), "Invalid mcx root directory provided");
        s_mcxRootDirectory = path;

        if (path.empty()) {
            return;
        }

        if (s_mcxRootDirectory.at(s_mcxRootDirectory.size() - 1) != '/') {
            s_mcxRootDirectory += '/';
        }
    }

    Shared<ClassicWindow> McxEngine::parseWindowFile(const std::string& path) {
        auto fullPath = s_mcxRootDirectory + path;

        bool fileExists = std::filesystem::is_regular_file(fullPath);
        if (!fileExists) {
            printf("'%s' could not be found\n", fullPath.c_str());
            return nullptr;
        }

        rapidxml::file<> xmlFile(fullPath.c_str());
        return parseWindowSource(xmlFile.data());
    }

    Shared<ClassicWindow> McxEngine::parseWindowSource(char* source) {
        if (!s_mcxEngineInitialized) {
            _initializeMcxEngine();
        }

        Shared<ClassicWindow> window = nullptr;

        rapidxml::xml_document<> doc;
        doc.parse<0>(source);

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

        if (!rootWidgetInstance) {
            printf("Failed to parse mcx window source\n");
            return nullptr;
        }

        if (!rootWidgetInstance->isContainer()) {
            printf("Root widget instance must be a BaseContainerWidget\n");
            return window;
        }

        window->setRootWidget(std::static_pointer_cast<BaseContainerWidget>(rootWidgetInstance));
        return window;
    }

    Shared<BaseWidget> McxEngine::parseUserWidgetFile(const std::string& path) {
        auto fullPath = s_mcxRootDirectory + path;

        bool fileExists = std::filesystem::is_regular_file(fullPath);
        if (!fileExists) {
            printf("'%s' could not be found\n", fullPath.c_str());
            return nullptr;
        }

        rapidxml::file<> xmlFile(fullPath.c_str());
        return parseUserWidgetSource(xmlFile.data());
    }

    Shared<BaseContainerWidget> McxEngine::parseUserWidgetFileAsContainer(const std::string& path) {
        auto fullPath = s_mcxRootDirectory + path;

        bool fileExists = std::filesystem::is_regular_file(fullPath);
        if (!fileExists) {
            printf("'%s' could not be found\n", fullPath.c_str());
            return nullptr;
        }

        rapidxml::file<> xmlFile(fullPath.c_str());
        auto widgetInstance = parseUserWidgetSource(xmlFile.data());

        if (!widgetInstance) {
            printf("Failed to parse user widget file '%s'\n", fullPath.c_str());
            return nullptr;
        }

        if (!widgetInstance->isContainer()) {
            return nullptr;
        }

        return std::static_pointer_cast<BaseContainerWidget>(
            widgetInstance
        );
    }

    Shared<BaseWidget> McxEngine::parseUserWidgetSource(char* source) {
        if (!s_mcxEngineInitialized) {
            _initializeMcxEngine();
        }

        rapidxml::xml_document<> doc;
        doc.parse<0>(source);

        auto rootNode = MakeRef<McxNode>(doc.first_node());
        const auto rootType = rootNode->getAttribute("type");

        // Making sure that the type of the root node is a window
        if (rootType != "widget") {
            printf("McxEngineError: root node is not a widget, '%s' type found\n",
                rootType.c_str());
            return nullptr;
        }

        auto& children = rootNode->getChildren();
        if (children.size() != 1) {
            printf("Custom user widget must have only one root widget\n");
            return nullptr;
        }

        auto& rootWidgetNode = children.at(0);

        Shared<BaseWidget> widgetInstance = parseWidget(rootWidgetNode);
        return widgetInstance;
    }

    void McxEngine::loadStylesheetFile(const std::string& path) {
        auto fullPath = s_mcxRootDirectory + path;

        bool fileExists = std::filesystem::is_regular_file(fullPath);
        if (!fileExists) {
            printf("'%s' could not be found\n", fullPath.c_str());
            return;
        }

        rapidxml::file<> xmlFile(fullPath.c_str());
        loadStylesheetSource(xmlFile.data());
    }

    void McxEngine::loadStylesheetSource(char* source) {
        if (!s_mcxEngineInitialized) {
            _initializeMcxEngine();
        }

        rapidxml::xml_document<> doc;
        doc.parse<0>(source);

        auto rootNode = MakeRef<McxNode>(doc.first_node());
        const auto rootType = rootNode->getAttribute("type");

        // Making sure that the type of the root node is a stylesheet
        if (rootType != "stylesheet") {
            printf("McxEngineError: root node is not a stylesheet, '%s' type found\n",
                rootType.c_str());

            return;
        }

        // Load each specified style
        for (auto& styleNode : rootNode->getChildren()) {
            auto styleName = styleNode->getAttribute("name");
            if (!styleName.empty()) {
                s_loadedStyles[styleName] = styleNode;
            }
        }
    }

    Shared<BaseWidget> McxEngine::parseWidget(Shared<McxNode>& node) {
        // Get the appropriate parsing adapter
        const auto mcxAdapter = getMcxAdapter(node->getType());
        if (!mcxAdapter) {
            throw std::runtime_error("Could not find adapter for type: " + node->getType());
        }

        // Create the native widget instance
        auto widgetInstance = mcxAdapter->createWidgetInstance(node);

        // Check if additional style properties needs to be injected
        auto styleString = node->getAttribute("style");
        if (!styleString.empty()) {
            // Tokenize the styles to get each style
            std::istringstream iss(styleString);
            std::vector<std::string> styles(std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>());

            // Iterate over each style and attempt to inject it
            for (const auto& style : styles) {
                injectStyle(style, node);
            }
        }

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

        // If there are any locally-defined styles,
        // they have to be removed after parsing the children.
        std::vector<std::string> localStyles;

        for (auto& child : childNodes) {
            // Check if a node is a local style
            if (child->getType() == "style") {
                auto styleName = child->getAttribute("name");
                if (!styleName.empty()) {
                    s_loadedStyles[styleName] = child;
                    localStyles.push_back(styleName);
                }

                continue;
            }

            auto childWidgetInstance = parseWidget(child);

            // Add the parsed child widget instance to this instance
            auto container = std::static_pointer_cast<BaseContainerWidget>(widgetInstance);
            container->addChild(childWidgetInstance);
        }

        // Remove any locally-defined styles
        for (auto& style : localStyles) {
            removeStyle(style);
        }

        return widgetInstance;
    }

    void McxEngine::injectStyle(const std::string& style, Shared<McxNode>& node) {
        if (s_loadedStyles.find(style) == s_loadedStyles.end()) {
            printf("Unknown style specified: '%s'\n", style.c_str());
            return;
        }

        auto& styleNode = s_loadedStyles.at(style);
        for (auto& [name, value] : styleNode->getAttribs()) {
            // If the target node has this attribute already, keep it,
            // essentially overwriting the style value with that value.
            if (!node->hasAttribute(name)) {
                node->setAttribute(name, value);
            }
        }
    }

    void McxEngine::removeStyle(const std::string& style) {
        if (s_loadedStyles.find(style) != s_loadedStyles.end()) {
            s_loadedStyles.erase(style);
        }
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
        registerMcxAdapter("SplitterPanel", MakeRef<SplitterPanelMcxAdapter>());
        registerMcxAdapter("Label", MakeRef<LabelMcxAdapter>());
        registerMcxAdapter("Button", MakeRef<ButtonMcxAdapter>());
        registerMcxAdapter("Checkbox", MakeRef<CheckboxMcxAdapter>());
        registerMcxAdapter("Combobox", MakeRef<ComboboxMcxAdapter>());
        registerMcxAdapter("DropdownButton", MakeRef<DropdownButtonMcxAdapter>());
        registerMcxAdapter("MenuList", MakeRef<MenuListMcxAdapter>());
        registerMcxAdapter("Entry", MakeRef<EntryMcxAdapter>());
        registerMcxAdapter("ProgressBar", MakeRef<ProgressBarMcxAdapter>());
        registerMcxAdapter("TreeView", MakeRef<TreeViewMcxAdapter>());
        registerMcxAdapter("TabView", MakeRef<TabViewMcxAdapter>());
        registerMcxAdapter("Image", MakeRef<ImageMcxAdapter>());
        registerMcxAdapter("UserWidget", MakeRef<UserWidgetMcxAdapter>());

        registerMcxPrimitiveColorNames();

        s_mcxEngineInitialized = true;
    }
} // namespace mc::mcx
