#include "processor.h"
#include "text.h"

namespace resume {
    Attributes IXmlProcessor::get_attributes(const XmlNode& node) {
        Attributes attrs;

        for (auto& attr : optional_attrs) {
            attrs[attr] = node.attribute(attr.c_str()).as_string();
        }

        for (auto& attr : required_attrs) {
            std::string attr_value = node.attribute(attr.c_str()).as_string();
            if (!attr.empty() && attr_value.empty()) {
                throw std::runtime_error("[Error] Required attribute " + attr + " not found in " + node.name() + ".");
            }

            attrs[attr] = attr_value;
        }

        return attrs;
    }

    CustomXmlProcessor::CustomXmlProcessor(const XmlNode& node) {
        // Parse optional attributes
        for (auto option : split<';'>(node.attribute("Optional").as_string())) {
            add_optional(option);
        }

        for (auto option : node.child("Optional")) {
            add_optional(option.text().as_string());
        }

        // Parse required attributes
        for (auto option : split<';'>(node.attribute("Required").as_string())) {
            add_required(option);
        }

        for (auto option : node.child("Required")) {
            add_required(option.text().as_string());
        }

        // Add template
        xml_string_writer writer;
        for (auto child : node.children()) {
            child.print(writer);
            this->mstch_template += writer.result;
        }
    }

    std::string CustomXmlProcessor::render(const XmlNode & custom_node, std::string_view children)
    {
        Attributes attrs = this->get_attributes(custom_node);

        mstch::map context;
        for (auto&[key, val] : attrs) {
            context[key] = val;
        }

        for (auto& attr : optional_attrs) {
            if (attrs.find(attr) == attrs.end()) {
                context[attr] = false;
            }
            else {
                context[attr] = mstch::map({ { "Value", attrs.find(attr)->second } });
            }
        }

        context["Children"] = std::string(children);
        context["Text"] = std::string(custom_node.child_value());
        return mstch::render(this->mstch_template, context);
    }
}