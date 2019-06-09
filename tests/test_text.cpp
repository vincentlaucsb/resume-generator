#include "catch.hpp"
#include "text.h"

namespace resume {
    TEST_CASE("String Split Test", "[string_split]") {
        auto str = GENERATE(as<std::string> {},
            "Apples;Oranges;Strawberries;Grapefruits",
            "Apples;    Oranges; Strawberries; Grapefruits" // Allow whitespace between delimiters
        );

        SECTION("Split Test") {
            auto res = split<';'>(str);

            REQUIRE(res[0] == "Apples");
            REQUIRE(res[1] == "Oranges");
            REQUIRE(res[2] == "Strawberries");
            REQUIRE(res[3] == "Grapefruits");
        }
    }
}