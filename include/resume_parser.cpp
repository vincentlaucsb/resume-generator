#include "resume.h"

namespace resume {
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

        while (resume().remove_child("Stylesheet"));
    }

    void ResumeParser::parse_custom_tags() {
        auto custom_tags = resume().child("Templates");
        for (auto section : custom_tags) {
            std::cout << "Reading custom rule " << section.name() << std::endl;
            this->add_custom_rule(section.name(), CustomXmlProcessor(section));
        }

        // Remove once we're done
        resume().remove_child("Templates");
    }

    std::map<std::string, std::string> ResumeParser::get_partials() {
        std::map<std::string, std::string> ret;

        for (auto&[name, processor] : this->custom_processors) {
            ret[name] = processor.get_template();
        }

        return ret;
    }

    std::string ResumeParser::process_children(const XmlNode& node) {
        auto partials = this->get_partials();
        std::string ret;
        for (auto child : node.children()) {
            // Only process XML tags
            if (child.type() == pugi::xml_node_type::node_element) {
                // Look up associated rule for processing this node
                if (this->custom_processors.find(child.name()) != this->custom_processors.end()) {
                    std::cout << "Using custom rule " << child.name() << std::endl;
                    std::string processed_children = this->process_children(child);
                    ret += this->custom_processors.find(child.name())->second.render(child, partials, processed_children);
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

                    ret += html_node.ToString();
                }
            }
        }

        return ret;
    }
}