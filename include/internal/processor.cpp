#include "processor.h"

namespace resume {
    NodeList process_list(const XmlNode& node) {
        NodeList list;
        HtmlList html_list;
        list << html_list;
        return list;
    }

    NodeList process_item(const XmlNode& node) {
        NodeList list;
        list << CTML::Node("li", node.text().as_string());
        return list;
    }

    NodeList process_school(const XmlNode& node) {
        NodeList list;
        CTML::Node container("div");
        container << add_subheading(
            node.attribute("Name").as_string(),
            std::string(node.attribute("Degree").as_string()) + " -- " +
            node.attribute("GPA").as_string());
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