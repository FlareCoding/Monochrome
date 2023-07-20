#pragma once
#include <core/Core.h>
#include <vector>
#include <map>
#include <widgets/Color.h>

namespace mc::mcx {
class McxNode {
public:
    /// @brief Default empty constructor that doesn't parse an xml node
    McxNode() : d_node(nullptr) {}

    /// @brief Used to abstract away the process of
    /// reading and processing xml node elements in mcx.
    /// @param node Pointer to the xml node object
    explicit McxNode(const void* node);

    /// @brief Used to initialize an empty node with a name
    /// that could be used during the exporting cycle.
    /// @param name Name of the potential underlying xml tag name
    explicit McxNode(const std::string& name);

    /// @brief Determines if child nodes should get processed as children of a widget
    bool childrenHandled = false;

    /// @param name The name of the attribute
    /// @returns Whether or not an attribute exists on the mcx node
    bool hasAttribute(const std::string& name);

    /// @param name The name of the attribute
    /// @param value The new value of the attribute
    void setAttribute(const std::string& name, const std::string& value);

    /// @brief Permanently removes an attribute from the node
    /// @param name Name of the attribute
    void deleteAttribute(const std::string& name);

    /// @returns Name of the current xml node name/type
    inline std::string& getType() { return d_nodeName; }

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    /// invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or empty string otherwise
    std::string getAttribute(const std::string& name, const std::string& defaultValue = "");

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    /// invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    int32_t getIntAttribute(const std::string& name, int32_t defaultValue = 0);

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    /// invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    uint32_t getUIntAttribute(const std::string& name, uint32_t defaultValue = 0);

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    /// invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    Color getColorAttribute(
        const std::string& name,
        const Color& defaultValue = Color::darkGray
    );

    /// @param name Name of the attribute
    ///
    /// @param defaultValue If the attribute doesn't exist or has an
    /// invalid value, the default value will be substituted as a return value.
    ///
    /// @returns The value of the node's attribute or provided default value otherwise
    bool getBoolAttribute(
        const std::string& name,
        bool defaultValue = false
    );

    /// @brief Creates a native xml node and writes current list of attributes to it
    /// @param doc Pointer to the native xml document object used to create the xml node
    /// @param parentMcxNode Parent mcx node to which this node can be
    /// added as a child in the xml document, nullptr if its the root node.
    void exportToXmlDoc(void* doc, McxNode* parentMcxNode);

    /// @returns The map of all attributes present in the node
    inline std::map<std::string, std::string>& getAttribs() { return d_attribs; }

    /// @returns The list of all child xml nodes
    inline std::vector<Shared<McxNode>>& getChildren() { return d_children; }

    /// @brief Removes all attributes
    inline void removeAllAttributes() { d_attribs.clear(); }

    /// @brief Returns the node's xml name
    inline const std::string& getNodeName() const { return d_nodeName; }

    /// @brief Sets the node's xml name
    inline void setNodeName(const std::string& name) { d_nodeName = name; }

    /// @brief Used to retrieve the native xml node handle
    inline const void* __getNativeXmlNodeHandle() const { return d_node; }

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

    // Writes the current attributes to the xml node
    void _writeAttribs(void* doc);

    // Appends children nodes to the xml node
    void _exportChildrenToXmlDoc(void* doc);
};
} // namespace mc::mcx
