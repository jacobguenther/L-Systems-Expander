#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Parsenode.h"
using std::string;
#include <sstream>
using std::istringstream;

class Parser {
   public:
    Parser(const string& _w) : input(_w) {}
    shared_ptr<Parsenode> parse();

   private:
    istringstream input;

    void match(char x);
    char nextchar() { return char(input.peek()); }  //!!!check for eof?
    double number();
    [[noreturn]] void error(const string&);
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
    string getidname();
};

#endif
