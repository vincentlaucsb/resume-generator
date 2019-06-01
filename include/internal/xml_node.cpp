#include "xml_node.h"

namespace resume {
    XmlNode::XmlAttribute XmlNode::get_optional_attr(const std::string& attr) const {
        auto xml_attr = this->attribute(attr.c_str());
        if (xml_attr.empty()) {
            std::cout << "[Warning] Missing attribute " << attr << " in " << this->name() << std::endl;
        }

        return xml_attr;
    }
}