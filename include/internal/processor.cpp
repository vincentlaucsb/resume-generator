#include "processor.h"

namespace resume {
    NodeList process_list(const XmlNode& node) {
        NodeList list;
        HtmlList html_list;
        list << html_list;
        return list;
    }

    NodeList process_item(const XmlNode& node) {
        auto heading = node.get_optional_attr("Heading");

        NodeList list;
        CTML::Node list_item("li");

        if (!heading.empty()) {
            std::string heading_str = fmt::format("{}: ", heading.as_string());
            list_item.AppendChild(CTML::Node("b").AppendText(heading_str));
        }

        list << list_item;
        return list;
    }

    NodeList process_school(const XmlNode& node) {
        auto name = node.get_optional_attr("Name"),
            gpa = node.get_optional_attr("GPA"),
            degree = node.get_optional_attr("Degree");

        NodeList list;
        CTML::Node container("div");
        container << add_subheading(
            name.as_string(),
            std::string(degree.as_string()) + " -- " +
            gpa.as_string());
        list << container;
        return list;
    }

    NodeList process_subheading(const XmlNode& node) {
        return add_subheading(
            node.attribute("Title").as_string(),
            node.attribute("Subtitle").as_string());
    }

    NodeList add_subheading(const std::string& title, const std::string& subtitle) {
        NodeList list;
        list << CTML::Node("h3", title)
            << CTML::Node("p", subtitle);
        return list;
    }
}