#ifndef RULE_H
#define RULE_H

#include "Cmd.h"
#include "Lexer.h"
#include "Parser.h"
#include <list>
#include <memory>
#include <stdexcept>

class Rule {
    friend class Lsystem;
   public:
    void readruleoptions(Lexer &lex);
	void setcmds(Commands&& newcmds);//!!! pass copy move
    void evaluateExpressions(const Context &cc) const;
    double getLocalScale() const;
    const Commands & getCommands() const;
    bool doesNotDraw() const;
    bool drawsInvisibly() const;
    bool isFixed() const;
    bool shouldFix() const;
private:
    Commands _commands;
//    std::unique_ptr<Parsenode> _rectWidthExpression;
    std::unique_ptr<Parsenode> _localScaleExpression;
    bool _drawsInvisibly=false;
    bool _doesNotDraw=false;
    bool _isFixed=false;
    bool _shouldFix=true;
//    mutable double _rectWidth=0.05;
    mutable double _localScale=1.0;
};

using Ruletable = std::unordered_map<std::string, Rule >;
#endif
