#pragma once
#include <set>
#include "common.h"
#include "xml_node.h"

namespace resume {
    /**
    struct XmlProcessor {
        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;
        virtual NodeList generate_html(const XmlNode& node) = 0;
    };
    */
    
    NodeList process_list(const XmlNode& node);
    NodeList process_item(const XmlNode& node);
    NodeList process_school(const XmlNode& node);
    NodeList process_subheading(const XmlNode& node);

    NodeList add_subheading(const std::string& title, const std::string& subtitle);
}