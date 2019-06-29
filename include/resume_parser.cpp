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
                auto& [name, processor] = *rule;

                std::cout << "Using custom rule " << child_name << std::endl;
                std::string processed_children = this->process_children(child);
                values[child.name()] = processor.render(child, this->partials, processed_children);
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
            std::cout << "[Info] Overriding previously defined rule " << section_name << std::endl;
        }

        this->custom_processors[section_name] = proc;
    }

    void ResumeParser::parse_templates() {
        // First, look for <Templates> node in current resume
        auto custom_tags = resume().child("Templates");
        if (!custom_tags.empty()) {
            this->load_custom_tags(custom_tags);

            // Load HTML template
            this->html_template = internals::parse_template(custom_tags.child("Body"));

            // Remove once we're done
            resume().remove_child("Templates");
        }

        // Then, look in given template XML files
        for (auto& xml_file : this->template_xml) {
            pugi::xml_document templates;
            auto load_template_result = templates.load_file(xml_file.c_str());
            if (!(bool)(load_template_result)) {
                std::cout << "[Warning] Couldn't find " << xml_file << std::endl;
                continue;
            }
                
            std::cout << "[Info] Reading templates from " << xml_file << std::endl;
            custom_tags = templates.child("Templates");
            this->load_custom_tags(custom_tags);

            // Load or replace HTML template
            if (auto body = custom_tags.child("Body"); !body.empty()) {
                this->html_template = internals::parse_template(body);
            }
        }

        // Update partials
        this->partials = this->get_partials();
    }

    void ResumeParser::load_custom_tags(const XmlNode& custom_tags)
    {
        // Load custom tags
        for (auto section : custom_tags) {
            std::cout << "[Info] Reading custom rule " << section.name() << std::endl;
            this->add_custom_rule(section.name(), CustomXmlProcessor(section));
        }
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