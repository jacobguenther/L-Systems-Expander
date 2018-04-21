#include "catch.hpp"
#include "glutFunctions.hpp"
#include "Parser.h"
#include "Lsystem.h"

TEST_CASE( "Parsing expressions", "[Parser]" ) {
    auto p = Parser("0").parse();
    REQUIRE(p != nullptr);
    REQUIRE(p->eval({})==Approx(0));
    REQUIRE(Parser("4>2").parse()->eval({}));
    REQUIRE(!bool(Parser("2>4").parse()->eval({})));
    REQUIRE(Parser("1?1:2").parse()->eval({})==Approx(1));
    REQUIRE(Parser("0?1:2").parse()->eval({})==Approx(2));
}

TEST_CASE("Reading config file and creating lsystem","[Lsystem]") {

    readtheconfigfile();
    

}
