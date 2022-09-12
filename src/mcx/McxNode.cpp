#include "McxNode.h"
#include "rapidxml/rapidxml.hpp"
#include <inttypes.h>

using xml_node = rapidxml::xml_node<char>;
using xml_attrib = rapidxml::xml_attribute<char>;

namespace mc::mcx {
    McxNode::McxNode(const void* node) : d_node(node) {
        _readAttribs();
        _readChildren();
    }

    bool McxNode::hasAttribute(const std::string& name) {
        return (d_attribs.find(name) != d_attribs.end());
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

        try {
            uint8_t r = defaultValue.r;
            uint8_t g = defaultValue.g;
            uint8_t b = defaultValue.b;
            uint8_t a = defaultValue.a;

            sscanf(
                d_attribs.at(name).c_str(),
                "Color(%" SCNu8 ", %" SCNu8 ", %" SCNu8 ", %" SCNu8 ")",
                &r, &g, &b, &a
            );

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
} // namespace mc::mcx
