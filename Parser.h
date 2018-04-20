#ifndef PARSER_H
#define PARSER_H

#include "Parsenode.h"
#include <sstream>
#include <string>

class Parser {
   public:
	explicit Parser(const std::string& _w);
    ParsenodePtr parse();

   private:
    std::istringstream input;

    void match(char x);
	char nextchar();
    double number();
    [[noreturn]] void error(const std::string& /*text*/);
    ParsenodePtr value();
    ParsenodePtr ternop(ParsenodePtr pred);
    ParsenodePtr boolexpr();
    ParsenodePtr bexpr();
    ParsenodePtr bterm();
    ParsenodePtr morebterms(ParsenodePtr lhs);
    ParsenodePtr morebfactors(ParsenodePtr lhs);
    ParsenodePtr morebool(ParsenodePtr lhs);
    ParsenodePtr expr();
    ParsenodePtr moreterms(ParsenodePtr lhs);
    ParsenodePtr term();
    ParsenodePtr morefactors(ParsenodePtr lhs);
    ParsenodePtr factor();
    ParsenodePtr idexpr();
    std::string getidname();
};
#endif
