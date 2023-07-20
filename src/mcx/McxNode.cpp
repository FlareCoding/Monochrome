#include "McxNode.h"
#include "rapidxml/rapidxml.hpp"
#include <inttypes.h>
#include <string>

#ifdef MC_PLATFORM_WINDOWS
    #define _CRT_SECURE_NO_WARNINGS
#endif

using xml_doc = rapidxml::xml_document<char>;
using xml_node = rapidxml::xml_node<char>;
using xml_attrib = rapidxml::xml_attribute<char>;

namespace mc::mcx {
    McxNode::McxNode(const void* node) : d_node(node) {
        _readAttribs();
        _readChildren();
    }

    McxNode::McxNode(const std::string& name) : d_node(nullptr), d_nodeName(name) {}

    bool McxNode::hasAttribute(const std::string& name) {
        return (d_attribs.find(name) != d_attribs.end());
    }

    void McxNode::setAttribute(const std::string& name, const std::string& value) {
        d_attribs[name] = value;
    }

    void McxNode::deleteAttribute(const std::string& name) {
        if (hasAttribute(name)) {
            d_attribs.erase(name);
        }
    }

    std::string McxNode::getAttribute(
        const std::string& name,
        const std::string& defaultValue
    ) {
        if (d_attribs.find(name) != d_attribs.end()) {
            return d_attribs.at(name);
        }

        return defaultValue;
    }

    int32_t McxNode::getIntAttribute(const std::string& name, int32_t defaultValue) {
        if (d_attribs.find(name) == d_attribs.end()) {
            return defaultValue;
        }

        try {
            int32_t value = std::stoi(d_attribs.at(name));
            return value;
        } catch (...) {
            return defaultValue;
        }
    }

    uint32_t McxNode::getUIntAttribute(const std::string& name, uint32_t defaultValue) {
        if (d_attribs.find(name) == d_attribs.end()) {
            return defaultValue;
        }

        try {
            uint32_t value = static_cast<uint32_t>(std::stoul(d_attribs.at(name)));
            return value;
        } catch (...) {
            return defaultValue;
        }
    }

    Color McxNode::getColorAttribute(const std::string& name, const Color& defaultValue) {
        if (d_attribs.find(name) == d_attribs.end()) {
            return defaultValue;
        }

        auto& val = d_attribs.at(name);
        if (val.find("Color(") == std::string::npos) {
            auto customColor = getCustomColor(val);

            if (customColor != Color::transparent) {
                return customColor;
            } else if (val == "transparent") {
                return customColor;
            }
        }

        try {
            uint8_t r = defaultValue.r;
            uint8_t g = defaultValue.g;
            uint8_t b = defaultValue.b;
            uint8_t a = defaultValue.a;

#ifdef MC_PLATFORM_WINDOWS
            sscanf_s(
                val.c_str(),
                "Color(%" SCNu8 ", %" SCNu8 ", %" SCNu8 ", %" SCNu8 ")",
                &r, &g, &b, &a
            );
#else
            sscanf(
                val.c_str(),
                "Color(%" SCNu8 ", %" SCNu8 ", %" SCNu8 ", %" SCNu8 ")",
                &r, &g, &b, &a
            );
#endif

            return Color(r, g, b, a);
        } catch (...) {
            return defaultValue;
        }
    }

    bool McxNode::getBoolAttribute(const std::string& name, bool defaultValue) {
        if (d_attribs.find(name) == d_attribs.end()) {
            return defaultValue;
        }

        return (d_attribs.at(name) == "true");
    }

    void McxNode::exportToXmlDoc(void* doc, McxNode* parentMcxNode) {
        auto xmlDoc = static_cast<xml_doc*>(doc);
        d_node = xmlDoc->allocate_node(rapidxml::node_element, d_nodeName.c_str());

        // Apply attributes and construct the children tree
        _writeAttribs(doc);
        _exportChildrenToXmlDoc(doc);

        // Get the non-const version of the current node as native xml object
        auto thisConstXmlNode = static_cast<const xml_node*>(d_node);
        auto thisXmlNode = const_cast<xml_node*>(thisConstXmlNode);

        // Add this node to the parent if necessary
        if (parentMcxNode) {
            // Working around the const cast
            auto constParentXmlNode = static_cast<const xml_node*>(parentMcxNode->d_node);
            auto parentXmlNode = const_cast<xml_node*>(constParentXmlNode);

            parentXmlNode->append_node(thisXmlNode);
        }
    }

    void McxNode::_readAttribs() {
        auto xmlNode = static_cast<const xml_node*>(d_node);
        d_nodeName = std::string(xmlNode->name());

        for(auto attrib = xmlNode->first_attribute();
            attrib;
            attrib = attrib->next_attribute()
        ) {
            auto name = std::string(attrib->name());
            auto value = std::string(attrib->value());

            d_attribs.insert({ name, value });
        }
    }

    void McxNode::_readChildren() {
        auto xmlNode = static_cast<const xml_node*>(d_node);

        for (auto childNode = xmlNode->first_node();
            childNode;
            childNode = childNode->next_sibling()
        ) {
            auto childMcxNode = MakeRef<McxNode>(childNode);
            d_children.push_back(childMcxNode);
        }
    }

    void McxNode::_writeAttribs(void* doc) {
        auto xmlDoc = static_cast<xml_doc*>(doc);

        // Get the non-const version of the current node as native xml object
        auto thisConstXmlNode = static_cast<const xml_node*>(d_node);
        auto thisXmlNode = const_cast<xml_node*>(thisConstXmlNode);

        for (auto& [attribName, attribValue] : d_attribs) {
            // Allocate an xml attribute
            auto xmlAttrib = xmlDoc->allocate_attribute(attribName.c_str(), attribValue.c_str());

            // Append the xml attribute to the xml node
            thisXmlNode->append_attribute(xmlAttrib);
        }
    }

    void McxNode::_exportChildrenToXmlDoc(void* doc) {
        // Get the non-const version of the current node as native xml object
        auto thisConstXmlNode = static_cast<const xml_node*>(d_node);
        auto thisXmlNode = const_cast<xml_node*>(thisConstXmlNode);

        for (auto& childMcxNode : d_children) {
            childMcxNode->exportToXmlDoc(doc, this);
        }
    }
} // namespace mc::mcx
