#include "resume.h"

namespace resume {
    void HtmlGenerator::process_children(const XmlNode& node, CTML::Node& parent) {
        CTML::Node * prev_node = nullptr;
        for (auto& child : node.children()) {
            // Add text to section node
            if (child.type() == pugi::xml_node_type::node_pcdata) {
                parent.AppendText(child.text().as_string());
            }

            // Only process XML tags
            if (child.type() == pugi::xml_node_type::node_element) {
                XmlProcessor processor;
                
                // Look up associated rule for processing this node
                if (this->try_get_rule(child.name(), processor)) {
                    for (auto& html_node : processor(child)) {
                        &(parent.AppendChild(html_node, prev_node));
                    }

                    this->process_children(child, *prev_node);
                }
            }
        }
    }
}