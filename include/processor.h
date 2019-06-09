#pragma once
#include <functional>
#include <unordered_map>
#include <set>
#include "common.h"
#include "xml_node.h"

namespace resume {
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
        NodeList process_node(const XmlNode& node);

    protected:
        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;

        virtual NodeList generate_html(Attributes& attrs) = 0;
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
        
    protected:
        virtual NodeList generate_html(Attributes& attrs) {
            return this->html_generator(attrs);
        }

        XmlRule html_generator;
    };

    class CustomXmlProcessor : public XmlProcessor {
    public:
        void set_html_template(const XmlNode& node) {
            this->html_template = std::make_unique<XmlNode>(node);
        }

    protected:
        virtual NodeList generate_html(Attributes& attrs) override {
            NodeList list;
            for (auto child : *html_template) {
                // Treat XML name as HTML tag name
                list << CTML::Node(child.name());
                this->process_html(attrs, child, list.back());
            }
            return list;
        }

    private:
        // XML describing HTML output
        std::unique_ptr<XmlNode> html_template = nullptr;

        // Recursive callback for generate_html()
        void process_html(Attributes& attrs, const XmlNode& node, CTML::Node& html);
    };

    NodeList process_subsection(Attributes& node);
    NodeList process_school(Attributes& attr);
    NodeList process_list(const Attributes& node);
    NodeList process_item(Attributes& node);

    NodeList add_subsection(std::string_view title, std::string_view right_text, std::string_view subtitle);
}