#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <functional>

#include "common.h"
#include "processor.h"
#include "text.h"

namespace resume {
    class H3 : public CTML::Node {
    public:
        H3() : CTML::Node("h3") {}
    };

    class ResumeParser {
    public:
        ResumeParser(const std::string& file) {
            this->result = doc.load_file(file.c_str());
        };

        // Returns True if XML loaded successfully
        bool ok() { return bool(result); }

        // Generate a resume
        std::string generate() {
            this->parse_templates();

            // Process resume data
            return this->process_resume(resume());
        }

        // Set the list of template XML files
        void set_template_xml(const std::vector<std::string>& files);

    private:
        // A map of processing rules
        std::unordered_map<std::string, CustomXmlProcessor> custom_processors = {};
        std::map<std::string, std::string> partials = {};

        // A list of template XML files to process
        std::vector<std::string> template_xml;

        pugi::xml_document doc;
        pugi::xml_parse_result result;
        std::string html_template;

        // Get the <Resume> node
        pugi::xml_node resume() {
            return doc.child("Resume");
        }

        // Read the XML data and generate a resume
        std::string process_resume(XmlNode node);

        // Add a rule for processing a custom XML tag
        void add_custom_rule(const std::string& section_name, const CustomXmlProcessor& proc);

        // Parse main template
        void parse_templates();

        // Helper for parse_templates() which parses an XML node containing a custom tag
        //
        // custom_tags: An XML node whose children are tag templates
        void load_custom_tags(const XmlNode& custom_tags);

        // Parse stylesheets
        void parse_stylesheets();

        // Create a mapping of partials
        std::map<std::string, std::string> get_partials();

        // Recursively process XML nodes and create HTML
        std::string process_children(const XmlNode& node);
    };
}