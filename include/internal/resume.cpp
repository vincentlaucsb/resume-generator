#include "resume.h"
#include <string>
#include <iostream>

resume::NodeList process_school(const resume::XmlNode& node) {
    resume::NodeList list;
    list.push_back(CTML::Node("h3", node.attribute("Name").as_string()));
    
    CTML::Node html_list("ul");

    for (auto item : node.children()) {
        html_list.AppendChild(CTML::Node("li", item.text().as_string()));
    }

    list.push_back(html_list);

    return list;
}

int main(int argc, char ** argv)
{
    using namespace resume;

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [input xml] [output html]" << std::endl;
        exit(1);
    }

    std::string filename = argv[1],
        output = argv[2];
    ResumeParser resume(filename);
    resume.gen.processors["School"] = process_school;

    if (!resume.ok()) {
        std::cout << "Couldn't load " << filename << std::endl;
        exit(1);
    }

    std::ofstream outfile(output);
    outfile << resume.generate() << std::endl;
}