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

    class XmlProcessor: public IXmlProcessor {
    public:
        XmlProcessor() = default;
        XmlProcessor(XmlRule rule, std::set<std::string> optional = {}, std::set<std::string> required = {}) :
            html_generator(rule), IXmlProcessor(optional, required)
        {};

        XmlProcessor& add_optional(std::string option) {
            this->optional_attrs.emplace(option);
            return *this;
        }
        
        NodeList generate_html(const XmlNode& node) {
            return this->html_generator(this->get_attributes(node));
        }

    protected:
        XmlRule html_generator;
    };

    class CustomXmlProcessor : public IXmlProcessor {
    public:
        CustomXmlProcessor() = default;
        CustomXmlProcessor(const XmlNode& node);

        void set_xml_template(const XmlNode& node) {
            std::string _template = "";
            xml_string_writer writer;

            for (auto child : node.children()) {
                child.print(writer);
                _template += writer.result;
            }

            this->xml_template = _template;
        }

        std::string generate_xml(XmlNode& custom_node);

    protected:
        // Given attributes, fill in the XML template with said attribute values
        // and convert the template to an XmlNode object
        std::string generate_xml(Attributes& attrs);

    private:
        // String template which consists of XML nodes that ResumeParser understands
        std::string xml_template;
    };

    NodeList process_subsection(Attributes& node);
    NodeList process_school(Attributes& attr);
    NodeList process_list(const Attributes& node);
    NodeList process_item(Attributes& node);

    NodeList add_subsection(std::string_view title, std::string_view right_text, std::string_view subtitle);
}