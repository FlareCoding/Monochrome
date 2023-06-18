#pragma once
#include <core/Core.h>
#include <vector>
#include <map>
#include <widgets/Color.h>

namespace mc::mcx {
class McxNode {
public:
    McxNode() = default;

    /// @brief Used to abstract away the process of
    /// reading and processing xml node elements in mcx.
    /// @param node Pointer to the xml node object
    explicit McxNode(const void* node);

    /// @brief Determines if child nodes should get processed as children of a widget
    bool childrenHandled = false;

    /// @param name The name of the attribute
    /// @returns Whether or not an attribute exists on the mcx node
    bool hasAttribute(const std::string& name);

    /// @param name The name of the attribute
    /// @param value The new value of the attribute
    void setAttribute(const std::string& name, const std::string& value);

    /// @returns Name of the current xml node name/type
    inline std::string& getType() { return d_nodeName; }

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    // invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or empty string otherwise
    std::string getAttribute(const std::string& name, const std::string& defaultValue = "");

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    // invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    int32_t getIntAttribute(const std::string& name, int32_t defaultValue = 0);

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    // invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    uint32_t getUIntAttribute(const std::string& name, uint32_t defaultValue = 0);

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    // invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    Color getColorAttribute(
        const std::string& name,
        const Color& defaultValue = Color::darkGray
    );

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    // invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    bool getBoolAttribute(
        const std::string& name,
        bool defaultValue = false
    );

    /// @returns The map of all attributes present in the node
    inline std::map<std::string, std::string>& getAttribs() { return d_attribs; }

    /// @returns The list of all child xml nodes
    inline std::vector<Shared<McxNode>>& getChildren() { return d_children; }

    /// @brief Removes all attributes
    inline void removeAllAttributes() { d_attribs.clear(); }

private:
    const void* d_node;
    std::string d_nodeName;

    std::map<std::string, std::string> d_attribs;
    std::vector<Shared<McxNode>> d_children;

private:
    // Parses and reads the xml node attributes
    void _readAttribs();

    // Parses children xml nodes
    void _readChildren();
};
} // namespace mc::mcx
