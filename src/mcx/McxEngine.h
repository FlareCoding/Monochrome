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

private:
    static bool s_mcxEngineInitialized;

    // Initializes the required mcx engine resources
    static void _initializeMcxEngine();

    // Used for parsing properties and applying them onto the widget instance
    static std::map<std::string, Shared<McxParsingAdapter>> s_mcxAdapters;
};
} // namespace mc::mcx
