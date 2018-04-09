#ifndef LEXER_H
#define LEXER_H
#include <iosfwd>
#include "Token.h"

using std::istream;

typedef enum {BSSPQ,BSSPN,NORMQ,NORMN,BSQ,BSN,EOLTOKEN,EOFTOKEN,DATATOKEN} State; 

class Lexer {
public:
   Lexer(istream &_in):infile(_in){}
   Token nexttoken();
   
private:
   void escapedalpha(char);
   State iws();
   State bsn();
   State bsq();
   State bsspn();
   State bsspq();
   State normn();
   State normq();

   istream & infile;
   string thetoken;
};

#endif

