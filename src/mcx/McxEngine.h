#pragma once
#include <window/ClassicWindow.h>
#include "adapters/McxParsingAdapter.h"

namespace mc::mcx {
class McxEngine {
public:
    /// @brief Parses an xml-based .mcx file containing a window as the root.
    /// @param path Path to the .mcx file containing the source
    /// @returns Shared pointer to the instantiated ClassicWindow object from the mcx source
    static Shared<ClassicWindow> parseWindowFile(const std::string& path);

    /// @brief Parses the xml-based mcx source code string with window as the root.
    /// @param source String containing mcx source code
    /// @returns Shared pointer to the instantiated ClassicWindow object from the mcx source
    static Shared<ClassicWindow> parseWindowSource(char* source);

    /// @brief Parses an xml-based .mcx file containing a custom widget as the root.
    /// @param path Path to the .mcx file containing the source
    /// @returns Shared pointer to the native widget instance from the mcx source
    static Shared<BaseWidget> parseUserWidgetFile(const std::string& path);

    /// @brief Parses an xml-based .mcx file containing a custom widget as the root.
    /// @param path Path to the .mcx file containing the source
    /// @returns Shared pointer to the native container widget instance from the mcx source
    static Shared<BaseContainerWidget> parseUserWidgetFileAsContainer(const std::string& path);

    /// @brief Parses the xml-based mcx source code string with custom widget as the root.
    /// @param source String containing mcx source code
    /// @returns Shared pointer to the native widget instance from the mcx source
    static Shared<BaseWidget> parseUserWidgetSource(char* source);

    /// @brief Parses an xml-based .mcx file containing properties of a custom style
    /// that can be reused by other widgets as their property, similar to CSS classes.
    /// @param path Path to the .mcx file containing the source
    static void loadStylesheetFile(const std::string& path);

    /// @brief Parses an xml-based mcx source code string containing properties of a custom
    /// style that can be reused by other widgets as their property, similar to CSS classes.
    /// @param source String containing mcx source code
    static void loadStylesheetSource(char* source);

    /// @brief Associates the mcx parsing adapter with a given widget name.
    /// Each widget has to have a corresponding adapter registerd.
    /// @param name Name of the widget type (e.g. StackPanel, Button, Checkbox, etc.)
    /// @param adapter Adapter instance to be used when parsing the widget's mcx source
    static void registerMcxAdapter(const std::string& name, Shared<McxParsingAdapter> adapter);

    /// @param widgetType Widget class name to get the adapter for
    /// @returns Returns the mcx parsing adapter instance
    /// that is associated with the given widget type
    static Shared<McxParsingAdapter> getMcxAdapter(const std::string& widgetType);

    /// @brief Parses the mcx source and produces a native widget instance
    /// @param node Node to the widget that needs to be parsed
    /// @returns Shared pointer to a native widget instance
    static Shared<BaseWidget> parseWidget(Shared<McxNode>& node);

    /// @brief Injects the given style's attributes into a provided node
    /// @param style String name of the style to be injected
    /// @param node Target node into which to inject the style attributes
    static void injectStyle(const std::string& style, Shared<McxNode>& node);

    /// @brief Sets the root directory for all mcx files that will be loaded
    static void setRootMcxDirectory(const std::string& path);

private:
    static bool s_mcxEngineInitialized;

    // Initializes the required mcx engine resources
    static void _initializeMcxEngine();

    // Used for parsing properties and applying them onto the widget instance
    static std::map<std::string, Shared<McxParsingAdapter>> s_mcxAdapters;

    // Map of all loaded styles
    static std::map<std::string, Shared<McxNode>> s_loadedStyles;

    // Used for organizing collections of mcx files in a project and
    // making it easier to reference them from within each other.
    static std::string s_mcxRootDirectory;
};
} // namespace mc::mcx
