#include "catch.hpp"
#include "text.h"

namespace resume {
    TEST_CASE("String Split Test", "[string_split]") {
        std::string test("Apples;Oranges;Strawberries;Grapefruits");
        auto res = split<';'>(test);

        REQUIRE(res[0] == "Apples");
        REQUIRE(res[1] == "Oranges");
        REQUIRE(res[2] == "Strawberries");
        REQUIRE(res[3] == "Grapefruits");
    }
}