#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <functional>

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

        void add_custom_rule(const std::string& section_name, CustomXmlProcessor * proc) {
            this->processors[section_name] = std::unique_ptr<IXmlProcessor>(proc);
        }

        void add_rule(const std::string& section_name, XmlRule func, std::set<std::string> optional = {}, std::set<std::string> required = {}) {
            this->processors[section_name] = std::unique_ptr<IXmlProcessor>(new XmlProcessor(
                func,
                optional,
                required
            ));
        }

        std::string get_html() {
            return this->html_document.ToString();
        }

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

        // Recursively process XML nodes and create HTML
        void process_children(const XmlNode& node, CTML::Node& parent);

        // Try to get a rule or print out a warning message
        bool try_get_rule(const std::string& name, IXmlProcessor *& out) {
            if (this->processors.find(name) != this->processors.end()) {
                out = this->processors[name].get();
                return true;
            }

            std::cout << "[Warning] No rule found for section " << name << "... skipping" << std::endl;
            return false;
        }

        std::unordered_map<std::string, std::unique_ptr<IXmlProcessor>> processors = {};
    };
}