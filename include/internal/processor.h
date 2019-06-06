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

        IXmlProcessor& add_optional(std::string option) {
            this->optional_attrs.emplace(option);
            return *this;
        }

        IXmlProcessor& add_required(std::string option) {
            this->required_attrs.emplace(option);
            return *this;
        }

        NodeList process_node(const XmlNode& node) {
            for (auto& attr : optional_attrs) {
                attrs[attr] = node.get_optional_attr(attr).as_string();
            }

            for (auto& attr : required_attrs) {
                attrs[attr] = node.attribute(attr.c_str()).as_string();
                if (attrs[attr].empty()) {
                    throw std::runtime_error("Required attribute " + attr + "not found.");
                }
            }

            return this->generate_html();
        }

    protected:
        std::unordered_map<std::string, std::string> attrs;
        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;

        virtual NodeList generate_html() = 0;
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

        NodeList process_node(const XmlNode& node) {
            for (auto& attr : optional_attrs) {
                attrs[attr] = node.get_optional_attr(attr).as_string();
            }

            for (auto& attr : required_attrs) {
                attrs[attr] = node.attribute(attr.c_str()).as_string();
                if (attrs[attr].empty()) {
                    throw std::runtime_error("Required attribute " + attr + "not found.");
                }
            }

            return this->generate_html();
        }
        
    protected:
        virtual NodeList generate_html() {
            return this->html_generator(this->attrs);
        }

        XmlRule html_generator;
    };

    class CustomXmlProcessor : public XmlProcessor {
    public:
        void set_html_template(const XmlNode& node) {
            this->html_template = std::make_unique<XmlNode>(node);
        }

    protected:
        virtual NodeList generate_html() override {
            NodeList list;
            for (auto child : *html_template) {
                // Treat XML name as HTML tag name
                list << CTML::Node(child.name());
                this->process_html(child, list.back());
            }
            return list;
        }

    private:
        void process_html(const XmlNode& node, CTML::Node& html) {
            CTML::Node * last_child = nullptr;
            for (auto child : node.children()) {
                // Add text to section node
                if (child.type() == pugi::xml_node_type::node_pcdata) {
                    html.AppendText(child.text().as_string());
                }

                // Only process XML tags
                if (child.type() == pugi::xml_node_type::node_element) {
                    auto name = std::string(child.name());
                    if (name != "placeholder" && name != "Placeholder") {
                        html.AppendChild(CTML::Node(name), last_child);
                        this->process_html(child, *last_child);
                    }
                    else {
                        // Replace placeholder
                        auto attr_to_get = child.attribute("Value").as_string();
                        html.AppendText(this->attrs[attr_to_get]);
                    }
                }
                
            }
        };

        std::unique_ptr<XmlNode> html_template = nullptr;
    };

    NodeList process_subheading(Attributes& node);
    NodeList process_school(Attributes& attr);
    NodeList process_list(const Attributes& node);
    NodeList process_item(Attributes& node);

    NodeList add_subheading(const std::string& title, const std::string& subtitle);
}