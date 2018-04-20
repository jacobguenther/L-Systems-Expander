#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <iosfwd>

class Lexer {
public:
	explicit Lexer(std::istream& _in);
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

    void escapedalpha(int next);
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

void assertdatatoken(const Token &t);

#endif
