#pragma once
#include <functional>
#include <unordered_map>
#include <set>
#include "common.h"
#include "xml_node.h"

#define PROCESSOR(name, optional, required) class name : public XmlProcessor { \
    public: \
        name(): XmlProcessor(optional, required) {}; \
    private: \
        virtual NodeList generate_html() override; \
};

namespace resume {
    class XmlProcessor {
    public:
        XmlProcessor() = default;
        XmlProcessor(XmlRule rule, std::set<std::string> optional = {}, std::set<std::string> required = {}) :
            html_generator(rule), optional_attrs(optional), required_attrs(required)
        {};

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

            return this->html_generator(this->attrs);
        }
        
    protected:
        std::unordered_map<std::string, std::string> attrs;

    private:
        std::set<std::string> optional_attrs;
        std::set<std::string> required_attrs;
        XmlRule html_generator;
    };

    NodeList process_subheading(Attributes& node);
    NodeList process_school(Attributes& attr);
    NodeList process_list(const Attributes& node);
    NodeList process_item(Attributes& node);

    NodeList add_subheading(const std::string& title, const std::string& subtitle);
}