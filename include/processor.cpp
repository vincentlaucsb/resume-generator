#include "processor.h"
#include "text.h"

namespace resume {
    Attributes CustomXmlProcessor::get_attributes(const XmlNode& node) {
        Attributes attrs;

        for (auto& attr : optional_attrs) {
            std::string attr_value = node.attribute(attr.c_str()).as_string();
            if (!attr_value.empty())
                attrs[attr] = attr_value;
        }

        for (auto& attr : required_attrs) {
            std::string attr_value = node.attribute(attr.c_str()).as_string();
            if (!attr.empty() && attr_value.empty()) {
                throw std::runtime_error("[Error] Required attribute " + attr + " not found in " + node.name() + ".");
            }

            attrs[attr] = attr_value;
        }

        // Transform attributes
        mstch::map current_values = DEFAULT_CONTEXT;
        for (auto&[k, v] : attrs) {
            current_values[k] = v;
        }

        // Allow for optional attributes in transforms
        for (auto& attr : optional_attrs) {
            if (attrs.find(attr) == attrs.end()) {
                current_values[attr] = false;
            }
            else {
                current_values[attr] = mstch::map({ { attr, attrs.find(attr)->second } });
            }
        }

        for (auto&[k, v] : attr_transforms) {
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
                    optional_attrs.emplace(option);
                }
            }
            else if (attr_name == "Required") {
                // Parse required attributes
                for (auto option : split<';'>(attr.value())) {
                    required_attrs.emplace(option);
                }
            }
            else {
                // Attribute transformations
                attr_transforms[attr_name] = attr.value();
            }
        }

        // Add template
        this->mstch_template = internals::parse_template(node);
    }

    std::string CustomXmlProcessor::render(const XmlNode & custom_node, std::map<std::string, std::string> partials, std::string_view children)
    {
        Attributes attrs = this->get_attributes(custom_node);
        mstch::map context = DEFAULT_CONTEXT;

        for (auto&[key, val] : attrs) {
            context[key] = val;
        }

        for (auto& attr : optional_attrs) {
            if (attrs.find(attr) == attrs.end()) {
                context[attr] = false;
            }
            else {
                context[attr] = mstch::map({ { attr, attrs.find(attr)->second } });
            }
        }

        context["Children"] = std::string(children);
        context["Text"] = std::string(custom_node.child_value());
        return mstch::render(this->mstch_template, context, partials);
    }
}

std::string resume::internals::parse_template(const XmlNode & node)
{
    std::string mstch_template;
    xml_string_writer writer;
    for (auto child : node.children()) {
        if (child.type() == pugi::xml_node_type::node_cdata) {
            mstch_template += child.value();
        }
        else {
            child.print(writer);
            mstch_template += writer.result;
        }
    }

    return mstch_template;
}
