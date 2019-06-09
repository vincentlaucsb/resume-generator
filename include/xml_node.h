#pragma once
#include "common.h"

namespace resume {
    // An extension of pugi's xml_node class
    class XmlNode : public pugi::xml_node {
        using XmlAttribute = pugi::xml_attribute;

    public:
        XmlNode(pugi::xml_node& node) : pugi::xml_node(node.internal_object()) {};

        XmlAttribute get_optional_attr(const std::string& attr) const;
    };
}