#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "pugixml.hpp"
#include "ctml.hpp"
#include "fmt/format.h"

namespace resume {
    using NodeList = std::vector<CTML::Node>;
    using XmlNode = pugi::xml_node;

    class HtmlGenerator {
    public:
        HtmlGenerator() {};

        std::unordered_map < std::string, std::function<NodeList(XmlNode)>> processors = {};

        CTML::Node& add_section(const std::string& name) {
            CTML::Node node("h2");
            node.AppendText(name);
            return this->document.body().AppendChild(node);
        }

        std::string get_html() {
            return this->document.ToString();
        }

    private:
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
            this->parse_sections();
            return this->gen.get_html();

        }

        // Temporary
        HtmlGenerator gen;

    private:
        // The parsed resume
        pugi::xml_document doc;
        pugi::xml_parse_result result;

        void parse_sections() {
            for (auto section : doc.child("Resume").child("Body")) {
                const char * section_name = section.name();
                auto & section_node = this->gen.add_section(section_name);
                for (auto child : section.children()) {
                    // Only process XML tags
                    if (child.type() == pugi::xml_node_type::node_element) {
                        for (auto& node : this->gen.processors[child.name()](child)) {
                            section_node.AppendChild(node);
                        }
                    }
                }

                std::cout << "Read section " << section_name << std::endl;
            }
        }
    };
}