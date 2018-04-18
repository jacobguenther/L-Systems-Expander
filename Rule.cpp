#include "Rule.h"
#include "Cmd.h"
#include <exception>

void Rule::setdrawmethod(Method m) {
    switch (m) {
        case NORM:
            break;
        case DROP:
            if (!dropangle)
                dropangle = Parser("0").parse();
            if (!dropdistance)
                dropdistance = Parser("1").parse();
            break;
        case RECT:
            if (!rectwidth)
                rectwidth = Parser("0.1").parse();
            break;
        case INVIS:
        case MIDPT:
        case WRITE:
            break;
    }
    drawmethod = m;
}

void Rule::readruleoptions(Lexer &lex) {
    Token t = lex.nexttoken();
    
    while (t.isdata()) {
        if (t.getdata() == "drawmethod") {
            t = lex.nexttoken();
            assertdatatoken(t);
            if (t.getdata() == "drop")
                setdrawmethod(Rule::DROP);
            else if (t.getdata() == "normal")
                setdrawmethod(Rule::NORM);
            else if (t.getdata() == "rectangle")
                setdrawmethod(Rule::RECT);
            else if (t.getdata() == "invisible")
                setdrawmethod(Rule::INVIS);
            else if (t.getdata() == "midpoint")
                setdrawmethod(Rule::MIDPT);
            else if (t.getdata() == "write")
                setdrawmethod(Rule::WRITE);
            else
                throw std::runtime_error("Unexpected draw method " + t.getdata());
        } else if (t.getdata() == "dropangle") {
            t = lex.nexttoken();
            assertdatatoken(t);
            dropangle = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        } else if (t.getdata() == "dropdistance") {
            t = lex.nexttoken();
            assertdatatoken(t);
            dropdistance = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        } else if (t.getdata() == "rectwidth") {
            t = lex.nexttoken();
            assertdatatoken(t);
            rectwidth = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        }                                               //!!!fix similarity of last three elseifs
        else if (t.getdata() == "info") {
            t = lex.nexttoken();
            assertdatatoken(t);
            info = t.getdata();
        }  //!!!even this is pretty similar,
        else if (t.getdata() == "localscale") {
            t = lex.nexttoken();
            assertdatatoken(t);
            scalefac = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        } else
            throw std::runtime_error("Unexpected option " + t.getdata());
        t = lex.nexttoken();
    }
}

void Rule::calculateParameters(const Context& cc) {
    for (auto & cmd : cmds)
        cmd->cachevalue(cc);
    switch (drawmethod) {
        case DROP:
            cacheddropangle = dropangle->eval(cc) * M_PI / 180.0;  //!!! cache sin and cos instead?
            cacheddropdistance = dropdistance->eval(cc);
            break;
        case RECT:
            cachedrectwidth = rectwidth->eval(cc);
            break;
        case NORM:
        case INVIS:
        case MIDPT:
        case WRITE:
            break;
    }
    _localScale = scalefac ? scalefac->eval(cc) : 1;
}
