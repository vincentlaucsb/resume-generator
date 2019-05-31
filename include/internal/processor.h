#pragma once
#include "common.h"

namespace resume {
    NodeList process_list(const XmlNode& node);
    NodeList process_item(const XmlNode& node);
    NodeList process_school(const XmlNode& node);
    NodeList process_subheading(const XmlNode& node);

    NodeList add_subheading(const std::string& title, const std::string& subtitle);
}