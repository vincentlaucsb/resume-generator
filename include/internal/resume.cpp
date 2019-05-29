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
    ResumeParser resume(filename);
    resume.add_rule("School", process_school);

    if (!resume.ok()) {
        std::cout << "Couldn't load " << filename << std::endl;
        exit(1);
    }

    std::ofstream outfile(output);
    outfile << resume.generate() << std::endl;
}