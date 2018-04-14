#include "Lexer.h"
#include <cctype>
#include <iostream>
#include <string>
#include "Token.h"

using std::isspace;
using std::string;

//!!! Should Lexer us char or int? ifstream.get() returns int (for EOF)
// but ifstream.get(c) takes char &. (What happens if that gets the ASCII 255 char?)
// probably char is correct, and always test ifstream for failure after a get.
/*
The Lexer module implements the following:
Tokens are EOL (end of line), EOF (end of file) and DATA (any other
   group of characters seperated by non-EOL whitespace). Tokens are
   subject to these rules:
   
# is comment to end of line, unless in a string, or escaped by \
" characters make strings (DATA tokens with whitespace, etc. in them),
   unless escaped by \
\ followed by whitespace is eaten (similar to C preprocesser,
   except where that eats only \ followed by newline, we eat all
   following whitespace until next non-whitespace character.)
\ escapes anything non-alphabetic (isalpha()). That is, \#, for
   instance, comes through as #. In particular, \#, \\, \" have
   no special meaning, they just create #, \, and " characters.
\ followed by alphabetic is for creating newlines (\n), tabs (\t),
   etc. See Lexer::escapedalpha() to find what is implemented.
   
Usage: Call nexttoken().

Design: A modified DFA. Driven by the nexttoken(), which "knows"
what state the DFA is. Calls a subroutine to find the next state.
"modified" DFA because we don't return to nexttoken when we don't
have to (that is, when we stay in the same state), and because
final states don't have subroutines, just handlers in nexttoken().

States:
   IWS   - start state, eating Initial White Space
   BSSPQ - eating BackSpace whiteSPace inside Quotes
   BSSPN - eating BackSpace whiteSPace Not inside quotes
   NORMQ - NORMal mode in Quotes
   NORMN - NORMal mode Not in quotes
   BSQ   - just saw a BackSpace in Quotes
   BSN   - just saw a BackSpace Not in quotes
   EOLTOKEN  - final state, return EOL token
   EOFTOKEN  - final state, return EOF token
   DATATOKEN - final state, return built up DATA token
*/

void Lexer::escapedalpha(int next) {
    switch (next) {
        case 'n':
            thetoken.push_back('\n');
            break;
        case 't':
            thetoken.push_back('\t');
            break;
        default:;  //throw "undefined escape"?!!!
    }
}

Lexer::State Lexer::iws() {
    auto next = infile.get();
    while (isspace(next) && next != '\n') next = infile.get();
    if (next == '\\') return State::BSN;
    if (next == '#') {
        infile.ignore(9999, '\n');
        return EOLTOKEN;
    }
    if (next == '"') return NORMQ;
    if (next == '\n') return EOLTOKEN;
    if (next == std::char_traits<char>::eof()) return EOFTOKEN;
    //otherwise
    infile.putback(char(next));
    return NORMN;
}

Lexer::State Lexer::bsspq() {
    auto next = infile.get();
    while (isspace(next)) next = infile.get();
    if (next == '\\') return BSQ;
    if (next == '"') return NORMN;
    if (next == std::char_traits<char>::eof()) return DATATOKEN;
    //otherwise (including #)
    thetoken.push_back(char(next));
    return NORMQ;
}

Lexer::State Lexer::bsspn() {
    auto next = infile.get();
    while (isspace(next)) next = infile.get();
    if (next == '\\') return BSN;
    if (next == '#') {
        infile.ignore(9999, '\n');
        ;
        infile.putback('\n');
        return DATATOKEN;
    }
    if (next == '"') return NORMQ;
    if (next == std::char_traits<char>::eof()) return DATATOKEN;
    //otherwise
    thetoken.push_back(char(next));
    return NORMN;
}

Lexer::State Lexer::normq() {
    while (true) {
        auto next = infile.get();
        if (next == std::char_traits<char>::eof()) return DATATOKEN;
        if (next == '\\') return BSQ;
        if (next == '"') return NORMN;
        if (next == '\n') {
            infile.putback('\n');
            return DATATOKEN;
        }  //throw "newline in string"?!!!
        //otherwise (including #)
        thetoken.push_back(char(next));
    }
}

Lexer::State Lexer::normn() {
    while (true) {
        auto next = infile.get();
        if (next == std::char_traits<char>::eof()) return DATATOKEN;
        if (next == '\\') return BSN;
        if (next == '#') {
            infile.ignore(9999, '\n');
            infile.putback('\n');
            return DATATOKEN;
        }
        if (next == '"') return NORMQ;
        if (next == '\n') {
            infile.putback('\n');
            return DATATOKEN;
        }  //throw "newline in string"?!!!
        if (isspace(next)) return DATATOKEN;
        //otherwise
        thetoken.push_back(char(next));
    }
}

Lexer::State Lexer::bsq() {
    auto next = infile.get();
    if (next == std::char_traits<char>::eof()) return DATATOKEN;
    if (isspace(next)) return BSSPQ;
    if (isalpha(next)) {
        escapedalpha(next);
        return NORMQ;
    }
    //otherwise
    thetoken.push_back(char(next));
    return NORMQ;
}

Lexer::State Lexer::bsn() {
    auto next = infile.get();
    if (next == std::char_traits<char>::eof()) return DATATOKEN;
    if (isspace(next)) return BSSPN;
    if (isalpha(next)) {
        escapedalpha(next);
        return NORMN;
    }
    //otherwise
    thetoken.push_back(char(next));
    return NORMN;
}

Token Lexer::nexttoken() {
    thetoken.clear();
    State thestate = iws();
    while (true) {
        switch (thestate) {
            case BSSPQ:
                thestate = bsspq();
                break;
            case BSSPN:
                thestate = bsspn();
                break;
            case NORMQ:
                thestate = normq();
                break;
            case NORMN:
                thestate = normn();
                break;
            case BSQ:
                thestate = bsq();
                break;
            case BSN:
                thestate = bsn();
                break;
            case EOLTOKEN:
                return Token(string(), Token::EOLT);
            case EOFTOKEN:
                return Token(string(), Token::EOFT);
            case DATATOKEN:
                return Token(thetoken, Token::DATAT);
        }
    }
}
