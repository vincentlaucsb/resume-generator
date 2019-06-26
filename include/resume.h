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

        std::string generate() {
            this->parse_template();
            this->parse_custom_tags();

            // Process resume data
            return this->process_resume(resume());
        }

    private:
        // A map of processing rules
        std::unordered_map<std::string, CustomXmlProcessor> custom_processors = {};

        pugi::xml_document doc;
        pugi::xml_parse_result result;
        std::string html_template;

        // Get the <Resume> node
        pugi::xml_node resume() {
            return doc.child("Resume");
        }

        std::string process_resume(XmlNode node) {
            auto partials = this->get_partials();
            std::string ret;
            mstch::map values;
            values["link"] = mstch::lambda{ [](const std::string& url) -> mstch::node {
    return fmt::format("<a href=\"{}\">{}</a>", url, url);
} };
            values["stylesheet"] = mstch::lambda{ [](const std::string& url) -> mstch::node {
                return fmt::format("<link href=\"{}\" rel=\"stylesheet\" type=\"text/css\"/>", url);
            } };

            // Convert all <Resume> attributes to usable variables
            for (auto attr : resume().attributes()) {
                values[attr.name()] = std::string(attr.value());
            }

            // Fill in mstch::map with recursively computed
            // values for top-level entries
            for (auto child : node.children()) {
                std::string child_name = child.name();
                if (auto rule = this->custom_processors.find(child_name); rule != this->custom_processors.end()) {
                    std::cout << "Using custom rule " << child_name << std::endl;
                    std::string processed_children = this->process_children(child);
                    values[child.name()] = rule->second.render(child, partials, processed_children);
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

        void add_custom_rule(const std::string& section_name, const CustomXmlProcessor& proc) {
            if (this->custom_processors.find(section_name) != this->custom_processors.end()) {
                throw std::runtime_error("[Error] Rule for " + section_name + " already exists.");
            }

            this->custom_processors[section_name] = proc;
        }

        // Parse main template
        void parse_template();

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