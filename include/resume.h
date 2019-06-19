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
        bool ok() {
            return bool(result);
        }

        std::string generate() {
            this->set_title(resume().attribute("Title").as_string());
            this->parse_stylesheets();
            this->parse_custom_tags();

            // Process resume data
            this->process_resume(resume());

            return this->get_html();
        }

        // Get the <Resume> node
        pugi::xml_node resume() {
            return doc.child("Resume");
        }

        void process_resume(XmlNode node) {
            auto & body = this->html_document.body();
            std::string body_text = this->process_children(node);
            
            // Do some text processing
            dashify(body_text);
            url(body_text);

            // Append text to body
            body.AppendText(body_text);
        }

        void set_title(const std::string& text) {
            this->html_document.AppendNodeToHead(
                CTML::Node("title", text)
            );
        }

        void add_custom_rule(const std::string& section_name, const CustomXmlProcessor& proc) {
            if (this->custom_processors.find(section_name) != this->custom_processors.end()) {
                throw std::runtime_error("[Error] Rule for " + section_name + " already exists.");
            }

            this->custom_processors[section_name] = proc;
        }

        std::string get_html() {
            return this->html_document.ToString();
        }

    private:
        // A map of processing rules
        std::unordered_map<std::string, CustomXmlProcessor> custom_processors = {};

        pugi::xml_document doc;
        CTML::Document html_document;
        pugi::xml_parse_result result;

        // Parse stylesheets
        void parse_stylesheets();

        // Parse user-defined tags
        void parse_custom_tags();

        // Create a mapping of partials
        std::map<std::string, std::string> get_partials();

        // Recursively process XML nodes and create HTML
        std::string process_children(const XmlNode& node);
    };
}