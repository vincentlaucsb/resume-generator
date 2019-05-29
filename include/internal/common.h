#pragma once
#include <functional>
#include <vector>

#include "pugixml.hpp"
#include "ctml.hpp"
#include "fmt/format.h"

namespace resume {
    // A list of HTML nodes
    using NodeList = std::vector<CTML::Node>;
    using XmlNode = pugi::xml_node;

    // A function that takes in an XML node and spits out
    // HTML output
    using XmlProcessor = std::function<NodeList(XmlNode)>;

    inline NodeList& operator<<(NodeList& nodes, const CTML::Node& node) {
        nodes.push_back(node);
        return nodes;
    }

    inline NodeList& operator<<(NodeList& lhs, NodeList& rhs) {
        for (auto & item : rhs) {
            lhs.push_back(item);
        }

        return lhs;
    }

    // An HTML <ul> element
    class HtmlList : public CTML::Node {
    public:
        HtmlList() : CTML::Node("ul") {}

        HtmlList& operator<<(const std::string& text) {
            this->AppendChild(CTML::Node("li", text));
            return *this;
        }
    };
}