#include "processor.h"
#include "text.h"

namespace resume {
    Attributes CustomXmlProcessor::get_attributes(const XmlNode& node) {
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

        mstch::map current_values;
        for (auto&[k, v] : attrs) {
            current_values[k] = v;
        }

        for (auto&[k, v] : attr_transforms) {
            std::cout << "Setting " << k << " to " << mstch::render(v, current_values) << std::endl;
            attrs[k] = mstch::render(v, current_values);
        }

        return attrs;
    }

    CustomXmlProcessor::CustomXmlProcessor(const XmlNode& node) {
        for (auto attr : node.attributes()) {
            std::string attr_name = attr.name();
            if (attr_name == "Optional") {
                // Parse optional attributes
                for (auto option : split<';'>(attr.value())) {
                    add_optional(option);
                }
            }
            else if (attr_name == "Required") {
                // Parse required attributes
                for (auto option : split<';'>(attr.value())) {
                    add_required(option);
                }
            }
            else {
                // Attribute transformations
                std::cout << "Registering attribute transformation: " << attr_name << " - " << attr.value() << std::endl;
                attr_transforms[attr_name] = attr.value();
            }
        }

        // Add template
        xml_string_writer writer;
        for (auto child : node.children()) {
            child.print(writer);
            this->mstch_template += writer.result;
        }
    }

    std::string CustomXmlProcessor::render(const XmlNode & custom_node, std::map<std::string, std::string> partials, std::string_view children)
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
        return mstch::render(this->mstch_template, context, partials);
    }
}