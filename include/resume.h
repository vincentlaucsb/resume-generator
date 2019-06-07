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
        HtmlGenerator() {
        };

        void process_resume(XmlNode node) {
            auto & body = this->document.body();
            this->process_children(node, body);
        }

        void set_title(const std::string& text) {
            this->document.AppendNodeToHead(
                CTML::Node("title", text)
            );
        }

        void add_custom_rule(const std::string& section_name, CustomXmlProcessor * proc) {
            this->processors[section_name] = std::unique_ptr<IXmlProcessor>(proc);
        }

        void add_rule(const std::string& section_name, XmlRule func, std::set<std::string> optional, std::set<std::string> required) {
            this->processors[section_name] = std::unique_ptr<IXmlProcessor>(new XmlProcessor(
                func,
                optional,
                required
            ));
        }

        std::string get_html() {
            return this->document.ToString();
        }

        CTML::Document document;
    private:


        // Recursively process XML nodes and create HTML
        void process_children(const XmlNode& node, CTML::Node& parent);

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
            this->parse_stylesheets();
            this->parse_custom_tags();
            this->parse_sections();
            return this->gen.get_html();
        }

        // Get the <Resume> node
        pugi::xml_node resume() {
            return doc.child("Resume");
        }

        void add_rule(const std::string& section_name,
            XmlRule func,
            std::set<std::string> optional = {},
            std::set<std::string> required = {}) {
            this->gen.add_rule(section_name, func, optional, required);
        }

    private:
        // The parsed resume
        HtmlGenerator gen;
        pugi::xml_document doc;
        pugi::xml_parse_result result;

        void set_title() {
            this->gen.set_title(
                resume().child("Title").text().as_string()
            );
        }

        void parse_stylesheets() {
            for (auto style : this->resume().children("Stylesheet")) {
                this->gen.document.AppendNodeToHead(
                    CTML::Node("link")
                    .SetAttribute("rel", "stylesheet")
                    .SetAttribute("type", "text/css")
                    .SetAttribute("href", style.text().as_string())
                );
            }

            this->resume().remove_child("Stylesheet");
            this->resume().remove_child("Stylesheet");
        }

        // Parse user-defined tags
        void parse_custom_tags();

        // Parse the different sections of the body
        void parse_sections() {
            this->gen.process_resume(resume());
        }
    };
}