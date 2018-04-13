#include "Parser.h"
#include <stdexcept>
#include "Parsenode.h"

using std::logic_error;
using std::runtime_error;

shared_ptr<Parsenode> Parser::parse() {
    Parsenode *val = value();
    if (input.get() == std::char_traits<char>::eof())
        return shared_ptr<Parsenode>(val);
    error("Expected end of input");  //throws
}

void Parser::match(char x) {     //should never be called unless you KNOW it's going to succeed
    char y = char(input.get());  //!!!Check for EOF? (probably not given precondition) CMH 4/8/18
    if (x != y)
        throw logic_error(string("Called match(") + x + ") but got " + y + "!");
}

Parsenode *Parser::value() {
    Parsenode *pred = boolexpr();
    if (nextchar() == '?') {
        match('?');
        Parsenode *trueval = value();
        if (nextchar() != ':') error("Expected : in ternary operator");
        match(':');
        Parsenode *falseval = value();
        return new Ternopnode(pred, trueval, falseval);
    }
    return pred;
}

Parsenode *Parser::boolexpr() {
    Parsenode *lhs = bexpr();
    Parsenode *more = morebool(lhs);
    if (more)
        return more;
    else
        return lhs;
}

Parsenode *Parser::morebool(Parsenode *lhs)  //clean up??!!!
{
    char op1 = nextchar();
    if (op1 == '<') {
        match(op1);
        char op2 = nextchar();
        if (op2 == '=') {
            match(op2);
            Parsenode *rhs = bexpr();
            return new Binopnode('l', lhs, rhs);  // l means <=
        } else {
            Parsenode *rhs = bexpr();
            return new Binopnode('<', lhs, rhs);
        }
    } else if (op1 == '>') {
        match(op1);
        char op2 = nextchar();
        if (op2 == '=') {
            match(op2);
            Parsenode *rhs = bexpr();
            return new Binopnode('g', lhs, rhs);  // g means >=
        } else {
            Parsenode *rhs = bexpr();
            return new Binopnode('>', lhs, rhs);
        }
    } else if (op1 == '=') {
        match(op1);
        if (nextchar() != '=') error("Expected = (for == operator)");
        match('=');
        Parsenode *rhs = bexpr();
        return new Binopnode('=', lhs, rhs);
    } else if (op1 == '!') {
        match(op1);
        if (nextchar() != '=') error("Expected = (for != operator)");
        match('=');
        Parsenode *rhs = bexpr();
        return new Binopnode('n', lhs, rhs);  // n means !=
    } else
        return 0;
}

Parsenode *Parser::bexpr() {
    Parsenode *lhs = bterm();
    while (nextchar() == '|') {
        match('|');
        if (nextchar() != '|') error("Expected | (for || operator)");
        match('|');
        Parsenode *rhs = bterm();
        lhs = new Binopnode('|', lhs, rhs);
    }
    return lhs;
}

Parsenode *Parser::bterm() {
    Parsenode *lhs = expr();
    while (nextchar() == '&') {
        match('&');
        if (nextchar() != '&') error("Expected & (for && operator)");
        match('&');
        Parsenode *rhs = expr();
        lhs = new Binopnode('&', lhs, rhs);
    }
    return lhs;
}

Parsenode *Parser::factor() {
    char next = nextchar();
    if (next == '-' || next == '!' || next == '+') {
        match(next);
        return new Unopnode(next, factor());
    }
    if (next == '(') {
        match('(');
        Parsenode *val = value();
        if (nextchar() != ')') error(") expected");
        match(')');
        return val;
    }

    if (isalpha(nextchar()) || nextchar() == '$')
        return idexpr();

    if (isdigit(nextchar()) || nextchar() == '.')
        return new Numnode(number());

    error("Expected expression");  //throws
    //return 0;
}

Parsenode *Parser::expr() {
    Parsenode *lhs = term();
    while (nextchar() == '+' ||
           nextchar() == '-') {
        char addop = nextchar();
        match(addop);
        Parsenode *rhs = term();
        lhs = new Binopnode(addop, lhs, rhs);
    }
    return lhs;
}

Parsenode *Parser::term() {
    Parsenode *lhs = factor();

    while (nextchar() == '*' ||
           nextchar() == '/' ||
           nextchar() == '%') {
        char mulop = nextchar();
        match(mulop);
        Parsenode *rhs = factor();
        lhs = new Binopnode(mulop, lhs, rhs);
    }
    return lhs;
}

Parsenode *Parser::idexpr() {
    string idname = getidname();
    vector<Parsenode *> params;
    if (nextchar() == '(') {
        match('(');
        while (1) {
            params.push_back(value());
            if (nextchar() == ',')
                match(',');
            else if (nextchar() == ')')
                break;
            else
                error("invalid character in parameter list");
        }
        match(')');
    }
    return new Idnode(idname, params);
}

string Parser::getidname() {
    string name;
    while (isalnum(nextchar()) || nextchar() == '$') {
        name.push_back(nextchar());
        match(nextchar());
    }
    return name;
}

double Parser::number() {
    double val;
    input >> val;
    return val;
}

void Parser::error(const string &text) {
    throw runtime_error(input.str() + "\n" +
                        string(input.eof() ? input.str().length() : static_cast<unsigned int>(input.tellg()), '-') + "^\n" + text);
}
/*
value -> boolexpr ternop
ternop -> ? value : value
       -> eps

boolexpr -> bexpr morebool

morebool -> boolop bexpr
		   -> eps

bexpr -> bterm morebterms
morebterms -> || bterm
           -> eps

bterm -> expr morebfactors
morebfactors -> && bfactor morebfactors
             -> eps

expr -> term moreterms
moreterms -> + term moreterms
          -> - term moreterms
          -> eps

term -> factor morefactors
morefactors -> * factor morefactors
            -> / factor morefactors
            -> % factor morefactors
            -> eps

factor -> - factor
       -> ! factor
       -> + factor
       -> ( value )
       -> idexpr
       -> number
       
idexpr -> id paramlist
paramlist -> ( params )
          -> eps
params -> value , params 
       -> value
       
*/
