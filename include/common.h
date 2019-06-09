#pragma once
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "pugixml.hpp"
#include "ctml.hpp"
#include "fmt/format.h"

namespace resume {
    // A list of HTML nodes
    using NodeList = std::vector<CTML::Node>;

    // Forward declaration
    using XmlNode = pugi::xml_node;

    using Attributes = std::unordered_map<std::string, std::string>;

    // A function that takes in resume attributes and spits out
    // HTML output
    using XmlRule = std::function<NodeList(Attributes)>;

    // Allows saving XML documents to strings
    struct xml_string_writer : pugi::xml_writer
    {
        std::string result;

        virtual void write(const void* data, size_t size)
        {
            result.append(static_cast<const char*>(data), size);
        }
    };

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

    inline CTML::Node& operator<<(CTML::Node& node, NodeList& nodes) {
        for (auto& child : nodes) {
            node.AppendChild(child);
        }

        return node;
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