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

    class HtmlGenerator {
    public:
        HtmlGenerator() {};

        void add_section(const XmlNode& section) {
            auto & body = this->document.body();
            body.AppendChild(CTML::Node("h2", split_camel_case(section.name())));
            this->process_children(section, body);
        }

        void set_title(const std::string& text) {
            this->document.AppendNodeToHead(
                CTML::Node("title", text)
            );
        }

        void add_rule(const std::string& section_name, XmlProcessor func) {
            this->processors[section_name] = func;
        }


        std::string get_html() {
            return this->document.ToString();
        }

    private:
        void process_children(const XmlNode& node, CTML::Node& parent) {
            CTML::Node * prev_node = nullptr;
            for (auto& child : node.children()) {
                // Add text to section node
                if (child.type() == pugi::xml_node_type::node_pcdata) {
                    parent.AppendText(child.text().as_string());
                }

                // Only process XML tags
                if (child.type() == pugi::xml_node_type::node_element) {
                    XmlProcessor processor;
                    if (this->try_get_rule(child.name(), processor)) {
                        for (auto& html_node : processor(child)) {
                            &(parent.AppendChild(html_node, prev_node));
                        }

                        this->process_children(child, *prev_node);
                    }
                }
            }
        }

        bool try_get_rule(const std::string& name, XmlProcessor& out) {
            if (this->processors.find(name) != this->processors.end()) {
                out = this->processors[name];
                return true;
            }

            std::cout << "[Warning] No rule found for section " << name << "... skipping" << std::endl;
            return false;
        }

        std::unordered_map<std::string, XmlProcessor> processors = {};
        CTML::Document document;
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
            this->set_title();
            this->parse_sections();
            return this->gen.get_html();
        }

        void add_rule(const std::string& name, XmlProcessor func) {
            this->gen.add_rule(name, func);
        }

    private:
        // The parsed resume
        HtmlGenerator gen;
        pugi::xml_document doc;
        pugi::xml_parse_result result;

        void set_title() {
            this->gen.set_title(
                doc.child("Resume").child("Title").text().as_string()
            );
        }

        // Parse the different sections of the body
        void parse_sections() {
            for (auto section : doc.child("Resume").child("Body")) {
                this->gen.add_section(section);
                std::cout << "Read section " << section.name() << std::endl;
            }
        }
    };
}