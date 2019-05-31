#pragma once
#include "ctml.hpp"

namespace resume {
    class HtmlNode : public CTML::Node {
        HtmlNode& AppendChildGetRef(HtmlNode& node) {
            this->AppendChild(node);
        }
    };
}