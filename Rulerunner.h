#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Turtle.h"
#include "Parser.h"
#include "Context.h"
#include "Lsystem.h"
#include "Graphic.h"

#include <stack>
using std::stack;
#include <list>
using std::list;
#include <map>
using std::map;
#include <string>
using std::string;
#include <stdexcept>
using std::logic_error;

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

class Rulestate
{
    friend class Rulerunner;
public:
    Rulestate(Rule * _m, bool _b, double _os, bool _f)
    :backwards(_b),flipped(_f),myrule(_m),
    mypos(backwards ? myrule->cmds.end() : myrule->cmds.begin()),oldscale(_os)
    {}
    
    bool done()
    {
        return mypos == (backwards ? myrule->cmds.begin() : myrule->cmds.end());
    }
    
    void doit(Rulerunner *towho);
private:
    bool backwards;
    bool flipped;
    Rule * myrule;
    Cmdcont::iterator mypos;
    double oldscale;
};

class Rulerunner
{
    friend class Rotatecmd; //!!!need friends, or make a public turtle accessor?
    friend class Flipcmd;
    friend class Popcmd;
    friend class Pushcmd;
    // friend class Rulecmd; // !!! similar question here, is handlerule() public?
public:
    Rulerunner(const Lsystem &_l, unsigned int _maxdepth, double _minscale, const Consttype & _c)
    :therules(_l.table),maxdepth(_maxdepth),finished(false),
    agraphic(),startrule(_l.startrule),
    context(_c,_l.expressions),backwards(false),minscale(_minscale)
    {
        for(Ruletable::iterator ii=therules.begin(); ii != therules.end(); ++ii)
            ii->second.cachevalues(context);
        turtles.push(Turtle());
        handlerule(startrule,false,false,1);
        makeapoint();
    }
    shared_ptr<Graphic> nextpoint();
    void drawnextpoint();
    bool done(){return finished;}
    void handlerule(const string &rr, bool rulerev, bool ruleflip, double localscale);
    void makeapoint();
    
    void graphic(const Motion &);
    ~Rulerunner(){Dropgraphic::haveapt=false;}
    //   void adjustcontext(const string &varname,double val)
    //      {context[varname]=val;} 
    
private:
    void pushturtle(){turtles.push(turtles.top());}
    void popturtle(){turtles.pop();}
    stack<Rulestate> rulestates;
    Ruletable therules;
    unsigned int maxdepth;
    stack<Turtle> turtles;
    bool finished;
    shared_ptr<Graphic> agraphic;
    string startrule;
    const Context context;
    bool backwards;
    double minscale;
};

#endif
