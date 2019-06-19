#pragma once
#include <functional>
#include <unordered_map>
#include <set>
#include "common.h"

namespace resume {
    namespace internals {
        // Parse an XML defined template
        std::string parse_template(const XmlNode& node);
    }

    class CustomXmlProcessor {
    public:
        CustomXmlProcessor() = default;
        CustomXmlProcessor(const XmlNode& node);

        // Grab the attributes from a node
        Attributes get_attributes(const XmlNode& node);

        // Gets the Mustache template associated with this tag
        const std::string& get_template() { return this->mstch_template; }

        // Given attributes, fill in the XML template with said attribute values
        // and convert the template to an XmlNode object
        std::string render(const XmlNode& custom_node, std::map<std::string, std::string> partials = {}, std::string_view children = "");

    private:
        // Mustache template
        std::string mstch_template = "";

        // Attribute transformations
        std::map<std::string, std::string> attr_transforms = {};

        // Optional attributes for this tag
        std::set<std::string> optional_attrs;

        // Required attributes for this tag
        std::set<std::string> required_attrs;
    };
}