#pragma once
#include <functional>
#include <unordered_map>
#include <set>
#include "common.h"

namespace resume {

    class ResumeParser;

    class IXmlProcessor {
    public:
        IXmlProcessor(std::set<std::string> optional = {}, std::set<std::string> required = {}) :
            optional_attrs(optional), required_attrs(required) {};

        IXmlProcessor& add_optional(std::string_view option) {
            this->optional_attrs.emplace(option);
            return *this;
        }

        IXmlProcessor& add_required(std::string_view option) {
            this->required_attrs.emplace(option);
            return *this;
        }

        // Grab the attributes from a node
        Attributes get_attributes(const XmlNode& node);

    protected:
        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;
    };

    class CustomXmlProcessor : public IXmlProcessor {
    public:
        CustomXmlProcessor() = default;
        CustomXmlProcessor(const XmlNode& node);

        const std::string& get_template() { return this->mstch_template;  }

        // Given attributes, fill in the XML template with said attribute values
        // and convert the template to an XmlNode object
        std::string render(const XmlNode& custom_node, std::map<std::string, std::string> partials = {}, std::string_view children = "");

    private:
        // Mustache template
        std::string mstch_template = "";
    };
}