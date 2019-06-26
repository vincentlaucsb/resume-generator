#include "resume.h"

namespace resume {
    void ResumeParser::parse_template()
    {
        auto templates = this->resume().child("Templates"),
            node = templates.child("Body");
        this->html_template = internals::parse_template(node);

        // Remove node once we're done
        templates.remove_child(node);
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
                std::string child_name = child.name();

                // Look up associated rule for processing this node
                if (auto rule = this->custom_processors.find(child_name); rule != this->custom_processors.end()) {
                    std::cout << "Using custom rule " << child_name << std::endl;
                    std::string processed_children = this->process_children(child);
                    ret += rule->second.render(child, partials, processed_children);
                }
                else {
                    std::cout << "[Warning] Couldn't find rule to process" << child_name << std::endl;
                }
            }
        }

        return ret;
    }
}