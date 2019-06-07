#include "resume.h"

namespace resume {
    const std::set<std::string> ResumeParser::ignore = { "Stylesheet", "CustomTags" };

    void ResumeParser::parse_stylesheets()
    {
        for (auto style : this->resume().children("Stylesheet")) {
            this->html_document.AppendNodeToHead(
                CTML::Node("link")
                .SetAttribute("rel", "stylesheet")
                .SetAttribute("type", "text/css")
                .SetAttribute("href", style.text().as_string())
            );
        }
    }

    void ResumeParser::parse_custom_tags() {
        auto custom_tags = resume().child("CustomTags");
        for (auto section : custom_tags) {
            std::cout << "Reading custom rule " << section.name() << std::endl;
            CustomXmlProcessor * custom_rule = new CustomXmlProcessor();

            // Parse optional attributes
            for (auto option : section.child("Optional")) {
                custom_rule->add_optional(option.text().as_string());
            }

            // Parse required attributes
            for (auto option : section.child("Required")) {
                custom_rule->add_required(option.text().as_string());
            }

            // Add template
            custom_rule->set_html_template(section.child("Template"));
            this->add_custom_rule(section.name(), custom_rule);
        }
    }

    void ResumeParser::process_children(const XmlNode& node, CTML::Node& parent) {
        CTML::Node * prev_node = nullptr;
        for (auto& child : node.children()) {
            if (this->ignore.find(child.name()) != this->ignore.end()) {
                // Ignore stylesheets, custom tags, etc..
                continue;
            }

            switch (child.type()) {
                // Add text to section node
            case pugi::xml_node_type::node_pcdata:
                parent.AppendText(child.text().as_string());
                break;

                // Only process XML tags
            case pugi::xml_node_type::node_element: {
                IXmlProcessor * processor = nullptr;

                // Look up associated rule for processing this node
                if (this->try_get_rule(child.name(), processor)) {
                    for (auto& html_node : processor->process_node(child)) {
                        &(parent.AppendChild(html_node, prev_node));
                    }

                    this->process_children(child, *prev_node);
                }

                break;
            }

            default:
                break;
            }
        }
    }
}