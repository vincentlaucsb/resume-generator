#pragma once
#include <unordered_map>
#include <vector>

#include "common.h"
#include "xml_node.h"

namespace resume {
    // Presents a user-defined XML tag
    class CustomTag {
        using AttributeValues = std::unordered_map<std::string, std::string>;

        // Convert custom  tag to an XML node that we recognize
        XmlNode generate(const AttributeValues& vals) {
            return this->generate(this->xml_template, vals);
        }

        XmlNode generate(const XmlNode& node, const AttributeValues& vals) {
            // Build up a fmtlib named argument list


            // Fill in {} arguments
            for (auto attr : node.attributes()) {

            }
        }

    private:
        XmlNode xml_template;
        std::vector<std::string> required_attributes = {};
        std::vector<std::string> optional_attributes = {};
    };
}