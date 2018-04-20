#ifndef RULE_H
#define RULE_H

#include "Lexer.h"
#include "Parser.h"
#include <list>
#include <memory>
#include <stdexcept>

class Command; //!!!rationalize includes, shouldn't need to forward declare here
using Commands = std::vector<std::shared_ptr<Command>>;


class Rule {
    friend class Turtle;
    friend class Rulestate;
    friend class Rulerunner;  //!!!be careful with friends?
    enum Method {
        NORM,
        DROP,
        RECT,
        INVIS,
        MIDPT,
        WRITE
    } ;
   public:
    void readruleoptions(Lexer &lex);
	void setcmds(Commands&& newcmds);
    //!!! initialize scalefac, or fix it later when syntax checking?
    void setdrawmethod(Method m);
    void calculateParameters(const Context &cc);

   private:
    Commands cmds;
    Method drawmethod=NORM;
    std::shared_ptr<Parsenode> _dropAngleExpression;
    std::shared_ptr<Parsenode> _dropDistanceExpression;
    std::shared_ptr<Parsenode> _rectWidthExpression;
    std::shared_ptr<Parsenode> _localScaleExpression;
    double _dropAngle=0.0;
    double _dropDistance=0.5;
    double _rectWidth=0.05;
    double _localScale=1.0;
    std::string _info;
};

using Ruletable = std::map<std::string, Rule >;
#endif
