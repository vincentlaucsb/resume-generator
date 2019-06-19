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
            this->process_text(resume());

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
            body.AppendText(this->process_children(node));
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

        // Convert URLs to <a> tags and so on
        void process_text(XmlNode node) {
            class my_walker: public pugi::xml_tree_walker
            {
            public:
                virtual bool begin(pugi::xml_node& node) override { return true;  }
                virtual bool end(pugi::xml_node& node) override { return true; }

                virtual bool for_each(pugi::xml_node& node) override {
                    // Perform text processings
                    for (auto& attr: node.attributes()) {
                        std::string attr_value = attr.value();
                        dashify(attr_value);
                        url(attr_value);
                        attr.set_value(attr_value.c_str());
                    }

                    return true;
                };
            };

            my_walker walker;
            node.traverse(walker);
        }

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