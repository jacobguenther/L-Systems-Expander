#ifndef PARSER_H
#define PARSER_H

#include "Parsenode.h"
#include "Lexer.h"
#include <sstream>
#include <string>

ParsenodePtr parse(const std::string &s);
ParsenodePtr parse(const Token &t);

class Parser {
friend ParsenodePtr parse(const std::string &s);
friend ParsenodePtr parse(const Token &t);

private:
    std::istringstream input;

    explicit Parser(const std::string& _w);
    ParsenodePtr parse();
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
