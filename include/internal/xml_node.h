#pragma once
#include "common.h"

namespace resume {
    // An extension of pugi's xml_node class
    class XmlNode : public pugi::xml_node {
        using XmlAttribute = pugi::xml_attribute;

    public:
        XmlNode(const pugi::xml_node& node) : pugi::xml_node(node.internal_object()) {};

        XmlAttribute get_optional_attr(const std::string& attr) const {
            auto xml_attr = this->attribute(attr.c_str());
            if (xml_attr.empty()) {
                std::cout << "[Warning] Missing attribute " << attr << " in " << this->name() << std::endl;
            }

            return xml_attr;
        }
    };
}