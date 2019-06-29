#include "cxxopts.hpp"
#include "resume.h"
#include <string>
#include <iostream>

using namespace resume;

int main(int argc, char ** argv)
{
    cxxopts::Options options(argv[0], "Generates a resume from XML input.");
    options.parse_positional({ "input" });
    options.add_options()
        ("input", "Input XML name", cxxopts::value<std::string>())
        ("o,output", "Output HTML name", cxxopts::value<std::string>()->default_value(""))
        ("t,template", "Template XML file", cxxopts::value<std::string>()->default_value("templates.xml"))
        ;

    if (argc < 2) {
        std::cout << options.help() << std::endl;
            exit(1);
    }

    try {
        auto result = options.parse(argc, argv);

        std::string input_xml = result["input"].as<std::string>(),
            template_xml = result["template"].as<std::string>(),
            output = result["output"].as<std::string>();

        if (output.empty()) {
            output = input_xml;
            output = replace(output, ".xml", ".html");
        }

        ResumeParser resume(input_xml.c_str());
        resume.template_xml = template_xml;

        if (!resume.ok()) {
            std::cout << "Couldn't load " << input_xml << std::endl;
            exit(1);
        }

        std::ofstream outfile(output);
        outfile << resume.generate() << std::endl;
        std::cout << "Resume generated in " << output << std::endl;
    }
    catch (std::exception& err) {
        std::cout << err.what() << std::endl;
        exit(1);
    }
}