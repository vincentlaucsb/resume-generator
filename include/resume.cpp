#include "cxxopts.hpp"
#include "resume.h"
#include <string>
#include <iostream>

using namespace resume;

int main(int argc, char ** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [input xml]" << std::endl;
        exit(1);
    }

    cxxopts::Options options("Resume Generator", "Generates a resume from XML input.");
    options.parse_positional({ "input" });
    options.add_options()
        ("input", "Input XML name", cxxopts::value<std::string>())
        ("o,output", "Output HTML name", cxxopts::value<std::string>()->default_value(""))
        ;

    try {
        auto result = options.parse(argc, argv);

        std::string input_xml = result["input"].as<std::string>(),
        output = result["output"].as<std::string>();

        if (output.empty()) {
            output = input_xml;
            output = replace(output, ".xml", ".html");
        }

        ResumeParser resume(input_xml.c_str());
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