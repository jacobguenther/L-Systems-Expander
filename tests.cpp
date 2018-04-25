#include "catch.hpp"
#include "GlutFunctions.hpp"
#include "Lsystem.h"
#include "Parser.h"

TEST_CASE( "Parsing expressions", "[Parser]" ) {
    auto p = parse("0");
    REQUIRE(p != nullptr);
    REQUIRE(p->eval({})==Approx(0));
    REQUIRE(parse("4>2")->eval({}));
    REQUIRE(!bool(parse("2>4")->eval({})));
    REQUIRE(parse("1?1:2")->eval({})==Approx(1));
    REQUIRE(parse("0?1:2")->eval({})==Approx(2));
}

TEST_CASE("Reading config file and creating lsystem","[Lsystem]") {

    readtheconfigfile();
    

}
