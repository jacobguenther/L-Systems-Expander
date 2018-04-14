#ifndef LEXER_H
#define LEXER_H
#include <iosfwd>
#include "Token.h"

class Lexer {

public:
    Lexer(std::istream &_in) : infile(_in) {}
    Token nexttoken();

   private:
    enum State { BSSPQ,
        BSSPN,
        NORMQ,
        NORMN,
        BSQ,
        BSN,
        EOLTOKEN,
        EOFTOKEN,
        DATATOKEN } ;

    void escapedalpha(int);
    State iws();
    State bsn();
    State bsq();
    State bsspn();
    State bsspq();
    State normn();
    State normq();

    std::istream &infile;
    std::string thetoken;
};

#endif
