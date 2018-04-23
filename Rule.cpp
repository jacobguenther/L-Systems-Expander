// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rule.h"
#include "Cmd.h"
#include <exception>

void Rule::setcmds(Commands&& newcmds) {
	_commands = std::move(newcmds);
}

void Rule::readruleoptions(Lexer &lex) {
    Token t = lex.nexttoken();
    
    while (t.isdata()) {
        if (t.getdata() == "invisible") { //!!!fix similarity of these
            _drawsInvisibly = true;
        } else if (t.getdata() == "nodraw") {
            _doesNotDraw=true;
        } else if (t.getdata() == "rectwidth") {
            t = lex.nexttoken();
            assertdatatoken(t);
            _rectWidthExpression = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        } else if (t.getdata() == "localscale") {
            t = lex.nexttoken();
            assertdatatoken(t);
            _localScaleExpression = Parser(t.getdata()).parse();  //!!! Rule member function? (then can take away friendship)
        } else
            throw std::runtime_error("Unexpected option " + t.getdata());
        t = lex.nexttoken();
    }
}

void Rule::evaluateExpressions(const Context& cc) const {
    for (auto & cmd : _commands)
        cmd->evaluateExpressions(cc);
    _localScale = _localScaleExpression ? _localScaleExpression->eval(cc) : 1.0;
}
