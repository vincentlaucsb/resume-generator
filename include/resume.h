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
    class CustomXmlProcessor;

    class H3 : public CTML::Node {
    public:
        H3() : CTML::Node("h3") {}
    };

    class ResumeParser {
    public:
        ResumeParser(const std::string& file) {
            this->result = doc.load_file(file.c_str());
        };

        // Recursively replace custom tags
        void process_custom_tags(XmlNode node);

        // Returns True if XML loaded successfully
        bool ok() {
            return bool(result);
        }

        std::string generate() {
            this->set_title(resume().attribute("Title").as_string());
            this->parse_stylesheets();
            this->parse_custom_tags();
            this->process_custom_tags(resume());
            this->process_resume(resume());
            return this->get_html();
        }

        // Get the <Resume> node
        pugi::xml_node resume() {
            return doc.child("Resume");
        }

        void process_resume(XmlNode node) {
            auto & body = this->html_document.body();
            this->process_children(node, body);
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

        void add_rule(const std::string& section_name, XmlRule func, std::set<std::string> optional = {}, std::set<std::string> required = {}) {
            this->processors[section_name] = XmlProcessor(
                func,
                optional,
                required
            );
        }

        std::string get_html() {
            return this->html_document.ToString();
        }

        friend CustomXmlProcessor;

    private:
        // Tags to ignore while generating HTML
        static const std::set<std::string> ignore;
        pugi::xml_document doc;
        CTML::Document html_document;
        pugi::xml_parse_result result;

        void set_title();

        // Parse stylesheets
        void parse_stylesheets();

        // Parse user-defined tags
        void parse_custom_tags();

        void process_custom_tags(XmlNode& node, XmlNode& parent_node);

        // Recursively process XML nodes and create HTML
        void process_children(XmlNode& node, CTML::Node& parent);

        // Try to get a rule or print out a warning message
        bool try_get_rule(const std::string& name, XmlProcessor& out);

        std::unordered_map<std::string, XmlProcessor> processors = {};
        std::unordered_map<std::string, CustomXmlProcessor> custom_processors = {};
    };
}