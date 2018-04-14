#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Parsenode.h"
#include <sstream>

class Parser {
   public:
    explicit Parser(const std::string& _w) : input(_w) {}
    std::shared_ptr<Parsenode> parse();

   private:
    std::istringstream input;

    void match(char x);
    char nextchar() { return char(input.peek()); }  //!!!check for eof?
    double number();
    [[noreturn]] void error(const std::string& /*text*/);
    Parsenode* value();
    Parsenode* ternop(Parsenode* pred);
    Parsenode* boolexpr();
    Parsenode* bexpr();
    Parsenode* bterm();
    Parsenode* morebterms(Parsenode* lhs);
    Parsenode* morebfactors(Parsenode* lhs);
    Parsenode* morebool(Parsenode* lhs);
    Parsenode* expr();
    Parsenode* moreterms(Parsenode* lhs);
    Parsenode* term();
    Parsenode* morefactors(Parsenode* lhs);
    Parsenode* factor();
    Parsenode* idexpr();
    std::string getidname();
};

#endif
