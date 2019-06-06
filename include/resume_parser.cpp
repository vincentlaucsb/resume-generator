#include "resume.h"

namespace resume {
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
            this->gen.add_custom_rule(section.name(), custom_rule);
        }

        // Remove tags now that we're done
        resume().remove_child("CustomTags");
    }
}