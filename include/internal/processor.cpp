#include "processor.h"

namespace resume {
    NodeList process_list(const XmlNode& node) {
        NodeList list;
        HtmlList html_list;
        for (auto item : node.children()) {
            // TODO: Assert items are of type "item"... or not?
            html_list << item.text().as_string();
        }
        list << html_list;
        return list;
    }

    NodeList process_school(const XmlNode& node) {
        NodeList list;
        list << CTML::Node("h3", node.attribute("Name").as_string());
        list << process_list(node.child("List"));
        return list;
    }

}