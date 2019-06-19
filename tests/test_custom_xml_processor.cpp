#include <iostream>
#include "catch.hpp"
#include "processor.h"

namespace resume {
    // Test that custom templates are parsed correctly
    TEST_CASE("Parse Template", "[parse_template]") {
        SECTION("Parse Mustache Template") {
            std::string item = ""
                "<Item Optional=\"Heading\">"
                "<li>"
                "{{#Heading}}"
                "<b>{{Value}}</b>"
                "{{/Heading}}"
                "{{Text}}"
                "</li>"
                "</Item>";

            pugi::xml_document doc;
            doc.load_string(item.c_str());

            auto item_template = doc.first_child();
            REQUIRE(internals::parse_template(item_template) == "<li>{{#Heading}}<b>{{Value}}</b>{{/Heading}}{{Text}}</li>\n");
        }

        SECTION("Parse Partial") {
            std::string job = ""
                "<Job"
                "    Required = \"Employer;Title\""
                "    Optional = \"Right\""
                "    Subtitle = \"{{Title}}\""
                "    Title = \"{{Employer}}\""
                ">"
                "    <![CDATA[{{> Subsection}}]]>"
                "    </Job>";

            pugi::xml_document doc;
            doc.load_string(job.c_str());

            auto job_template = doc.first_child();
            REQUIRE(internals::parse_template(job_template) == "{{> Subsection}}");
        }
    }
}