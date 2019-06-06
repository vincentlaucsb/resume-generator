#include "processor.h"
#include "text.h"

namespace resume {
    NodeList IXmlProcessor::process_node(const XmlNode& node) {
        for (auto& attr : optional_attrs) {
            attrs[attr] = node.get_optional_attr(attr).as_string();
        }

        for (auto& attr : required_attrs) {
            std::string attr_value = node.attribute(attr.c_str()).as_string();
            if (attr_value.empty()) {
                throw std::runtime_error("Required attribute " + attr + "not found.");
            }

            attrs[attr] = attr_value;
        }

        return this->generate_html();
    }

    void CustomXmlProcessor::process_html(const XmlNode& node, CTML::Node& html) {
        CTML::Node * last_child = nullptr;
        for (auto child : node.children()) {
            // Add text to section node
            if (child.type() == pugi::xml_node_type::node_pcdata) {
                html.AppendText(child.text().as_string());
            }

            // Only process XML tags
            if (child.type() == pugi::xml_node_type::node_element) {
                auto name = lower(child.name());
                if (name != "placeholder") {
                    html.AppendChild(CTML::Node(name), last_child);
                    this->process_html(child, *last_child);
                }
                else {
                    // Replace placeholder
                    auto attr_to_get = child.attribute("Value").as_string();
                    html.AppendText(this->attrs[attr_to_get]);
                }

                // TODO: Allow users to reference other defined tags
            }

        }
    }

    NodeList process_list(const Attributes& node) {
        NodeList list;
        HtmlList html_list;
        list << html_list;
        return list;
    }

    NodeList process_item(Attributes& attr) {
        auto heading = attr["Heading"];

        NodeList list;
        CTML::Node list_item("li");

        if (!heading.empty()) {
            std::string heading_str = fmt::format("{}: ", heading);
            list_item.AppendChild(CTML::Node("b").AppendText(heading_str));
        }

        list << list_item;
        return list;
    }

    NodeList process_school(Attributes& attr) {
        NodeList list;
        CTML::Node container("div");
        container << add_subheading(
            attr["Name"],
            attr["Degree"] + " -- " +
            attr["GPA"]);
        list << container;
        return list;
    }

    NodeList process_subheading(Attributes& attr) {
        return add_subheading(
            attr["Title"],
            attr["Subtitle"]);
    }

    NodeList add_subheading(const std::string& title, const std::string& subtitle) {
        NodeList list;
        list << CTML::Node("h3", title)
            << CTML::Node("p", subtitle);
        return list;
    }
}