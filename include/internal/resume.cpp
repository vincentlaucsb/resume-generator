#include "resume.h"
#include <string>
#include <iostream>

int main(int argc, char ** argv)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(argv[1]);
    if (!result)
        return -1;

    auto resume_xml = doc.child("Resume");
    auto resume_template = resume_xml.child("Template");

    auto src = resume_template.child("Body").attribute("src").as_string();
    std::ifstream body_src(src);

    std::string body_template = "";
    while (!body_src.eof()) {
        std::string temp;
        std::getline(body_src, temp);
        body_template += temp + '\n';
    }

    std::cout << body_template << std::endl;

    try {
        std::cout << fmt::format(body_template, fmt::arg("Address", "HI"), fmt::arg("Email", "yolo@swag.com")) << std::endl;
    }
    catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
    }

    /**
    for (pugi::xml_node node : doc)
    {
        resume << node.name() << std::endl;
        for (auto child : node.children()) {
            resume << child.text().as_string() << std::endl;
        }
    }
    **/
}