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
            this->add_custom_rule(section.name(), CustomXmlProcessor(section));
        }

        // Remove once we're done
        resume().remove_child("CustomTags");
    }

    bool ResumeParser::try_get_rule(const std::string& name, XmlProcessor& out)
    {
        if (this->processors.find(name) != this->processors.end()) {
            out = this->processors[name];
            return true;
        }

        return false;
    }

    void ResumeParser::process_children(XmlNode& node, CTML::Node& parent) {
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
                XmlProcessor processor;

                // Look up associated rule for processing this node
                if (this->try_get_rule(child.name(), processor)) {
                    std::cout << "Processing " << child.name() << std::endl;
                    for (auto& html_node : processor.generate_html(child)) {
                        parent.AppendChild(html_node, prev_node);
                    }
                }
                else if (this->custom_processors.find(child.name()) != this->custom_processors.end()) {
                    parent.AppendText(this->custom_processors.find(child.name())->second.render(child));
                    prev_node = &parent;
                }
                else {
                    // Treat as regular HTML tag if no hit
                    std::string tag_name = child.name();

                    // Strip out html_ prefix
                    if (tag_name.substr(0, 5) == "html_") {
                        tag_name = tag_name.substr(5);
                    }

                    std::cout << "Processing HTML tag " << tag_name << std::endl;

                    CTML::Node html_node(tag_name);
                    for (auto& attr : child.attributes()) {
                        html_node.SetAttribute(attr.name(), attr.as_string());
                    }

                    parent.AppendChild(html_node, prev_node);
                }

                // Recursively process children
                this->process_children(XmlNode(child), *prev_node);
                break;
            }

            default:
                break;
            }
        }
    }
}