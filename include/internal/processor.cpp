#include "processor.h"

namespace resume {
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