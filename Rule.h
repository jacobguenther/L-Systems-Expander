#ifndef RULE_H
#define RULE_H

#include <memory>
#include <stdexcept>
#include <list>

#include "Lexer.h"
class Cmd;
using Cmdcont = std::list<std::shared_ptr<Cmd> >; //!!!rationalize includes, shouldn't need to forward declare here
//and Cmdcont is defined again in Cmd.h, bad form!!!

#include "Parser.h"


class Rule {
    friend class Turtle;
    friend class Rulestate;
    friend class Rulerunner;  //!!!be careful with friends?
    friend void readruleoptions(Lexer &lex, Rule &r);
    enum Method { NORM,
                   DROP,
                   RECT,
                   INVIS,
                   MIDPT } ;
   public:
    void setcmds(Cmdcont &&newcmds) { cmds = std::move(newcmds); }
    //!!! initialize scalefac, or fix it later when syntax checking?
    void setdrawmethod(Method m);
    void cachevalues(const Context &cc);

   private:
    Cmdcont cmds;
    Method drawmethod=NORM;
    std::shared_ptr<Parsenode> dropangle;
    std::shared_ptr<Parsenode> dropdistance;
    std::shared_ptr<Parsenode> rectwidth;
    std::shared_ptr<Parsenode> scalefac;
    double cacheddropangle;
    double cacheddropdistance;
    double cachedrectwidth;
    double cachedscalefac;
    std::string info;
    //Color
};

using Ruletable = std::map<std::string, Rule >; //!!!Should be reference, not copy

#endif
