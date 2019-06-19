#pragma once
#include <functional>
#include <unordered_map>
#include <set>
#include "common.h"

namespace resume {

    class ResumeParser;

    class CustomXmlProcessor {
    public:
        CustomXmlProcessor() = default;
        CustomXmlProcessor(const XmlNode& node);

        CustomXmlProcessor& add_optional(std::string_view option) {
            this->optional_attrs.emplace(option);
            return *this;
        }

        CustomXmlProcessor& add_required(std::string_view option) {
            this->required_attrs.emplace(option);
            return *this;
        }

        // Grab the attributes from a node
        Attributes get_attributes(const XmlNode& node);

        const std::string& get_template() { return this->mstch_template;  }

        // Given attributes, fill in the XML template with said attribute values
        // and convert the template to an XmlNode object
        std::string render(const XmlNode& custom_node, std::map<std::string, std::string> partials = {}, std::string_view children = "");

    private:
        // Mustache template
        std::string mstch_template = "";

        // Attribute transformations
        std::map<std::string, std::string> attr_transforms = {};

        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;
    };
}