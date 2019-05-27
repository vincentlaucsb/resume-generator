#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "pugixml.hpp"
#include "ctml.hpp"
#include "fmt/format.h"

class HtmlGenerator {
public:
    HtmlGenerator() {};

    void add_section(const std::string& name) {
        CTML::Node node("h2");
        node.AppendText(name);
        this->document.AppendNodeToBody(node);
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
        for (auto sect : this->sections) {
            this->gen.add_section(sect);
        }

        return this->gen.get_html();
    }

private:
    // The parsed resume
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    HtmlGenerator gen;
    std::vector<std::string> sections = {};

    void parse_sections() {
        for (auto section : doc.child("Resume").child("Body")) {
            this->sections.push_back(section.name());
            std::cout << "Read section " << section.name() << std::endl;
        }
    }
};