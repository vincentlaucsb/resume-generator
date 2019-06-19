#include <iostream>
#include "catch.hpp"
#include "processor.h"

namespace resume {
    const char * job = ""
        "<Job"
        "    Required = \"Employer; Title\""
        "    Optional = \"Start; End\""
        "    Subtitle = \"{{Title}}\""
        "    Title = \"{{Employer}}\""
        "    Right = \"{{Start}} -- {{End}}\""
        ">"
        "    <![CDATA[{{> Subsection}}]]>"
        "</Job>";

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
            pugi::xml_document doc;
            doc.load_string(job);

            auto job_template = doc.first_child();
            REQUIRE(internals::parse_template(job_template) == "{{> Subsection}}");
        }
    }

    TEST_CASE("Attribute Transformations Test", "[test_attr_transform]") {
        pugi::xml_document doc;
        doc.load_string(job);

        auto job_template = doc.first_child();
        CustomXmlProcessor processor(job_template);

        std::string data = "<Job Employer=\"Raytheon Company\" Title=\"Software Engineer\" Start=\"January 2019\" End=\"Present\" />";
        pugi::xml_document data_doc;
        data_doc.load_string(data.c_str());
        auto data_node = data_doc.first_child();

        auto attrs = processor.get_attributes(data_node);
        REQUIRE(attrs["Title"] == "Raytheon Company");
        REQUIRE(attrs["Subtitle"] == "Software Engineer");
        REQUIRE(attrs["Right"] == "January 2019 -- Present");
    }
}