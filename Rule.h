#ifndef RULE_H
#define RULE_H

#include <memory>
#include <stdexcept>
#include <list>

#include "Lexer.h"
class Cmd;
#include "Parser.h"

using Cmdcont = std::list<std::shared_ptr<Cmd> >;

class Rule {
    friend class Rulestate;
    friend class Rulerunner;  //!!!be careful with friends?
    friend void readruleoptions(Lexer &lex, Rule &r);
    enum Method { NORM,
                   DROP,
                   RECT,
                   INVIS,
                   MIDPT } ;
   public:
    void setcmds(const Cmdcont &newcmds) { cmds = newcmds; }  //!!!need smart ptr here, delete old cmds
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

using Ruletable = std::map<std::string, Rule>;

#endif
