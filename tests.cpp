#include "catch.hpp"
#include "Parser.h"

TEST_CASE( "Parsing expressions", "[Parser]" ) {
    auto p = Parser("0").parse();
    REQUIRE(p != nullptr);
}
