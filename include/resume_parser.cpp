#include "resume.h"

namespace resume {
    std::string ResumeParser::process_resume(XmlNode node)
    {
        std::string ret;
        mstch::map values = DEFAULT_CONTEXT;

        // Convert all <Resume> attributes to usable variables
        for (auto attr : resume().attributes()) {
            values[attr.name()] = std::string(attr.value());
        }

        // Fill in mstch::map with recursively computed
        // values for top-level entries
        for (auto child : node.children()) {
            std::string child_name = child.name();
            if (auto rule = this->custom_processors.find(child_name); rule != this->custom_processors.end()) {
                std::cout << "Using custom rule " << child_name << std::endl;
                std::string processed_children = this->process_children(child);
                values[child.name()] = rule->second.render(child, this->partials, processed_children);
            }
            else {
                std::cout << "[Warning] Couldn't find rule to process" << child_name << std::endl;
            }
        }

        ret = mstch::render(this->html_template, values);

        // Do some text processing
        dashify(ret);

        return ret;
    }

    void ResumeParser::add_custom_rule(const std::string & section_name, const CustomXmlProcessor & proc)
    {
        if (this->custom_processors.find(section_name) != this->custom_processors.end()) {
            throw std::runtime_error("[Error] Rule for " + section_name + " already exists.");
        }

        this->custom_processors[section_name] = proc;
    }

    void ResumeParser::parse_templates() {
        auto custom_tags = resume().child("Templates");
        pugi::xml_document templates;
        auto load_template_result = templates.load_file("templates.xml");
        ("templates.xml");

        if (custom_tags.empty()) {
            std::cout << "[Info] Reading templates from templates.xml" << std::endl;
            if (!(bool)(load_template_result)) {
                std::cout << "[Warning] Couldn't find templates.xml" << std::endl;
            }
            else {
                custom_tags = templates.child("Templates");
            }
        }

        // Load HTML template
        this->html_template = internals::parse_template(custom_tags.child("Body"));

        // Load custom tags
        for (auto section : custom_tags) {
            std::cout << "Reading custom rule " << section.name() << std::endl;
            this->add_custom_rule(section.name(), CustomXmlProcessor(section));
        }

        // Update partials
        this->partials = this->get_partials();

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
        std::string ret;
        for (auto child : node.children()) {
            // Only process XML tags
            if (child.type() == pugi::xml_node_type::node_element) {
                std::string child_name = child.name();

                // Look up associated rule for processing this node
                if (auto rule = this->custom_processors.find(child_name); rule != this->custom_processors.end()) {
                    std::cout << "Using custom rule " << child_name << std::endl;
                    std::string processed_children = this->process_children(child);
                    ret += rule->second.render(child, this->partials, processed_children);
                }
                else {
                    std::cout << "[Warning] Couldn't find rule to process " << child_name << std::endl;
                }
            }
        }

        return ret;
    }
}