#include "resume.h"
#include <iostream>
#include <sstream>

int main(int argc, char ** argv)
{
    std::stringstream resume;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(argv[1]);
    if (!result)
        return -1;

    for (pugi::xml_node node : doc)
    {
        resume << node.name() << std::endl;
        for (auto child : node.children()) {
            resume << child.text().as_string() << std::endl;
        }
    }

    std::cout << resume.str() << std::endl;
}