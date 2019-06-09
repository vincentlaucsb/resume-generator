#include <iostream>
#include "catch.hpp"
#include "processor.h"

namespace resume {
    TEST_CASE("Custom XML Processor", "[custom_xml_proc]") {
        std::string xml_template_str = "<Header Optional=\"Address; Email; LinkedIn; Website\">"
            "<Template>"
            "<html_header>"
            "<h1 id=\"name\">Vincent La</h1>"
            "<p>"
            "<span>{Address}</span>"
            "<span>{Email}</span>"
            "<span>{LinkedIn}</span>"
            "<span>{Website}</span>"
            "</p>"
            "</html_header>"
            "</Template>"
            "</Header>";

        pugi::xml_document xml_doc;
        xml_doc.load_string(xml_template_str.c_str());
        XmlNode xml_template = xml_doc.child("Header");
        xml_doc.print(std::cout);

        pugi::xml_document custom_tag;
        custom_tag.load_string("<Header Email=\"spam@meat.com\" />");
        custom_tag.print(std::cout);

        CustomXmlProcessor processor(xml_template);
        std::cout << processor.generate_xml(XmlNode(custom_tag.child("Header")));
    }
}