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
            if (attr_value.empty()) {
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
            add_optional(option);
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

    std::string CustomXmlProcessor::render(XmlNode & custom_node)
    {
        return this->render(this->get_attributes(custom_node));
    }

    std::string CustomXmlProcessor::render(Attributes& attrs) {
        mstch::map context;
        for (auto&[key, val] : attrs) {
            context[key] = val;
        }

        return mstch::render(this->mstch_template, context);
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
        list << add_subsection(
            attr["Name"],
            attr["Years"],
            attr["Degree"] + " &ndash; " +
            attr["GPA"]);
        return list;
    }

    NodeList process_section(Attributes & attrs)
    {
        NodeList list;
        list << CTML::Node("div").AppendChild(CTML::Node("h2", attrs["Title"]));
        return list;
    }

    NodeList process_subsection(Attributes& attr) {
        return add_subsection(
            attr["Title"],
            attr["Right"],
            attr["Subtitle"]);
    }

    NodeList add_subsection(std::string_view title, std::string_view right_text, std::string_view subtitle) {
        NodeList list;
        CTML::Node _title = CTML::Node("h3", title.data());
        CTML::Node _right_text = CTML::Node("span", right_text.data());
        _right_text.SetAttribute("style", "float: right");
        _title.AppendChild(_right_text);

        list << CTML::Node("div")
            .SetAttribute("class", "subsection")
            .AppendChild(_title)
            .AppendChild(
                CTML::Node("p", subtitle.data())
                .SetAttribute("class", "subtitle")
            );
        return list;
    }
}