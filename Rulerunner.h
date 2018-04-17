#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Graphic.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Turtle.h"

#include <stack>
#include <list>
#include <map>
#include <string>
#include <stdexcept>

#include "Rule.h"

class Lexer;
class Rulerunner;

//class Rulestate
//Member variables are
//   Rule * myrule  = the currently running rule
//   bool backwards = is the currently running rule reversed
//   Cmdcont::iterator mypos = the position in the currently running rule
//   double oldscale= what was our scale before we started this rule
//   bool flipped   = did we flip the turtle when we started running this rule

class Rulestate {
    friend class Rulerunner;

   public:
    Rulestate(const Rule *_m, bool _b, double _os, bool _f)
        : backwards(_b), flipped(_f), myrule(_m), mypos(backwards ? myrule->cmds.cend() : myrule->cmds.cbegin()), oldscale(_os) {}

    bool done() {
        return mypos == (backwards ? myrule->cmds.begin() : myrule->cmds.end());
    }

    void doit(Rulerunner *towho);

   private:
    bool backwards;
    bool flipped;
    const Rule *myrule;
    Cmdcont::const_iterator mypos;
    double oldscale;
};

class Rulerunner {
    friend class Rotatecmd;  //!!!need friends, or make a public turtle accessor?
    friend class Flipcmd;
    friend class Popcmd;
    friend class Pushcmd;
    // friend class Rulecmd; // !!! similar question here, is handlerule() public?
   public:
    Rulerunner(const Lsystem &l, unsigned int maxdepth, double minscale, const Consttype &c)
    : _therules(l.table), _startrule(l.startrule)
    ,_rulestates{{Rulestate{&_therules[_startrule],false,1.0,false}}}
    ,_maxdepth(maxdepth),_context(c, l.expressions), _minscale(minscale)
    {
        Dropgraphic::haveapt = false;
        for (auto & therule : _therules)
            therule.second.cachevalues(_context);
//        handlerule(_startrule, false, false, 1);
        makeapoint();
    }
    std::shared_ptr<Graphic> nextpoint();
    void drawnextpoint();
    bool done() { return _finished; }
    void handlerule(const std::string &rr, bool rulerev, bool ruleflip, double localscale);
    bool isDeepEnough();
   private:
    void graphic(const Motion &);
    void makeapoint();
    
    Ruletable _therules;
    std::string _startrule;
    std::stack<Rulestate> _rulestates;
    unsigned int _maxdepth;
    Turtle _turtle;
    bool _finished=false;
    std::shared_ptr<Graphic> _agraphic;
    const Context _context;
    bool _backwards=false;
    double _minscale;
};

#endif
