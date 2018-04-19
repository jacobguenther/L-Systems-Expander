// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Parser.h"
#include <stdexcept>
#include "Parsenode.h"

using std::logic_error;
using std::runtime_error;
using std::vector;
using std::string;
using std::move;
using std::make_unique;

ParsenodePtr Parser::parse() {
    auto val = value();
    if (input.get() == std::char_traits<char>::eof())
        return val;
    error("Expected end of input");  //throws
}

void Parser::match(char x) {     //should never be called unless you KNOW it's going to succeed
    auto y = char(input.get());  //!!!Check for EOF? (probably not given precondition) CMH 4/8/18
    if (x != y)
        throw logic_error(string("Called match(") + x + ") but got " + y + "!");
}

ParsenodePtr Parser::value() {
    ParsenodePtr pred = bexpr();
    if (nextchar() == '?') {
        match('?');
        ParsenodePtr trueval = value();
        if (nextchar() != ':') error("Expected : in ternary operator");
        match(':');
        ParsenodePtr falseval = value();
        return make_unique<Ternopnode>(move(pred), move(trueval), move(falseval));
    }
    return pred;
}

ParsenodePtr Parser::morebool(ParsenodePtr lhs)  //clean up??!!!
{
    char op1 = nextchar();
    if (op1 == '<') {
        match(op1);
        char op2 = nextchar();
        if (op2 == '=') {
            match(op2);
            ParsenodePtr rhs = bexpr();
            return make_unique<Binopnode>('l', move(lhs), move(rhs)); // l means <=
        }
        ParsenodePtr rhs = bexpr();
        return make_unique<Binopnode>('<', move(lhs), move(rhs));
    }
    if (op1 == '>') {
        match(op1);
        char op2 = nextchar();
        if (op2 == '=') {
            match(op2);
            ParsenodePtr rhs = bexpr();
            return make_unique<Binopnode>('g', move(lhs), move(rhs));  // g means >=
        }
        ParsenodePtr rhs = bexpr();
        return make_unique<Binopnode>('>', move(lhs), move(rhs));
    }
    if (op1 == '=') {
        match(op1);
        if (nextchar() != '=') error("Expected = (for == operator)");
        match('=');
        ParsenodePtr rhs = bexpr();
        return make_unique<Binopnode>('=', move(lhs), move(rhs));
    }
    if (op1 == '!') {
        match(op1);
        if (nextchar() != '=') error("Expected = (for != operator)");
        match('=');
        ParsenodePtr rhs = bexpr();
        return make_unique<Binopnode>('n', move(lhs), move(rhs));  // n means !=
    }
    return lhs;
}

ParsenodePtr Parser::bexpr() {
    ParsenodePtr lhs = bterm();
    while (nextchar() == '|') {
        match('|');
        if (nextchar() != '|') error("Expected | (for || operator)");
        match('|');
        ParsenodePtr rhs = bterm();
        lhs = make_unique<Binopnode>('|', move(lhs), move(rhs));
    }
    return morebool(move(lhs));
}

ParsenodePtr Parser::bterm() {
    ParsenodePtr lhs = expr();
    while (nextchar() == '&') {
        match('&');
        if (nextchar() != '&') error("Expected & (for && operator)");
        match('&');
        ParsenodePtr rhs = expr();
        lhs = make_unique<Binopnode>('&', move(lhs), move(rhs));
    }
    return lhs;
}

ParsenodePtr Parser::factor() {
    char next = nextchar();
    if (next == '-' || next == '!' || next == '+') {
        match(next);
        return make_unique<Unopnode>(next, factor());
    }
    if (next == '(') {
        match('(');
        ParsenodePtr val = value();
        if (nextchar() != ')') error(") expected");
        match(')');
        return val;
    }

    if (isalpha(nextchar()) || nextchar() == '$')
        return idexpr();

    if (isdigit(nextchar()) || nextchar() == '.')
        return make_unique<Numnode>(number());

    error("Expected expression");
}

ParsenodePtr Parser::expr() {
    ParsenodePtr lhs = term();
    while (nextchar() == '+' ||
           nextchar() == '-') {
        char addop = nextchar();
        match(addop);
        ParsenodePtr rhs = term();
        lhs = make_unique<Binopnode>(addop, move(lhs), move(rhs));
    }
    return lhs;
}

ParsenodePtr Parser::term() {
    ParsenodePtr lhs = factor();

    while (nextchar() == '*' ||
           nextchar() == '/' ||
           nextchar() == '%') {
        char mulop = nextchar();
        match(mulop);
        ParsenodePtr rhs = factor();
        lhs = make_unique<Binopnode>(mulop, move(lhs), move(rhs));
    }
    return lhs;
}

ParsenodePtr Parser::idexpr() {
    std::string idname = getidname();
    vector<ParsenodePtr > params;
    if (nextchar() == '(') {
        match('(');
        while (true) {
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
    return make_unique<Idnode>(idname, move(params));
}

std::string Parser::getidname() {
    std::string name;
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

void Parser::error(const std::string &text) {
    throw runtime_error(input.str() + "\n" +
                        string(input.eof() ? input.str().length() : static_cast<unsigned int>(input.tellg()), '-') + "^\n" + text);
}
/*
value -> bexpr ternop
ternop -> ? value : value
       -> eps

bexpr -> bterm morebterms morebool
morebterms -> || bterm
           -> eps

morebool -> boolop bexpr
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
