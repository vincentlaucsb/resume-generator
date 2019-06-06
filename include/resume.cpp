#include "resume.h"
#include <string>
#include <iostream>

using namespace resume;

int main(int argc, char ** argv)
{
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " [input xml] [output html]" << std::endl;
        exit(1);
    }

    std::string filename = argv[1],
        output = argv[2];

    try {
        ResumeParser resume(filename);
        resume.add_rule("School", process_school, { "Name", "Years", "GPA", "Degree" });
        resume.add_rule("Subheading", process_subheading, { "Title", "Subtitle" });
        resume.add_rule("List", process_list);
        resume.add_rule("Item", process_item, { "Heading" });

        if (!resume.ok()) {
            std::cout << "Couldn't load " << filename << std::endl;
            exit(1);
        }

        std::ofstream outfile(output);
        outfile << resume.generate() << std::endl;
    }
    catch (std::runtime_error& err) {
        std::cout << err.what() << std::endl;
        exit(1);
    }
}