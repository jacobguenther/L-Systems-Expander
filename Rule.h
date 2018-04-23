#ifndef RULE_H
#define RULE_H

#include "Cmd.h"
#include "Lexer.h"
#include "Parser.h"
#include <list>
#include <memory>
#include <stdexcept>

class Rule {
    friend class Turtle;
    friend class Rulestate;
    friend class Rulerunner;  //!!!be careful with friends?
    friend class RuleCommand;
    friend class LinesDrawStrategy;
    friend class DropDrawStrategy; //!!! Fix these
    friend class DrawStrategy;
    //use accessors, or friend the base class and give it protected methods
    //or use a strategy here
   public:
    void readruleoptions(Lexer &lex);
	void setcmds(Commands&& newcmds);//!!! pass copy move
    void calculateParameters(const Context &cc) const;

   private:
    Commands _commands;
    std::unique_ptr<Parsenode> _rectWidthExpression;
    std::unique_ptr<Parsenode> _localScaleExpression;
    bool _drawsInvisibly=false;
    bool _doesNotDraw=false;
    mutable double _rectWidth=0.05;
    mutable double _localScale=1.0;
};

using Ruletable = std::map<std::string, Rule >;
#endif
