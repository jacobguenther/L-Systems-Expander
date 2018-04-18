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
//   bool isReversed = is the currently running rule reversed
//   double flipFactor = do we flip the turtle when we start running this rule
//   const Rule & myRule  = the currently running rule
//   Cmdcont::const_iterator _nextCommand = the next command to run
//   double scaleFactor = what was our scale before we started this rule

class Rulestate {
    friend class Rulerunner;

   public:
    Rulestate(const Rule &myRule, bool isReversed, double scaleFactor, double flipFactor)
        : _isReversed(isReversed), _flipFactor(flipFactor), _myRule(myRule)
        ,_nextCommand(_isReversed ? _myRule.cmds.cend() : _myRule.cmds.cbegin())
        , _scaleFactor(scaleFactor) {
    }

    bool hasNoMoreCommands() {
        return _nextCommand == (_isReversed ? _myRule.cmds.begin() : _myRule.cmds.end());
    }

    void runCurrentCommandOn(Rulerunner *towho);

   private:
    bool _isReversed;
    double _flipFactor;
    const Rule &_myRule;
    Commands::const_iterator _nextCommand;
    double _scaleFactor;
};

class Rulerunner {
friend class RotateCommand;  //!!!need friends, or make a public turtle accessor?
friend class FlipCommand;
friend class PopCommand;
friend class PushCommand;
friend class RuleCommand;
public:
    Rulerunner(const Lsystem &l, unsigned int maxdepth, double minscale, const Consttype &c)
    : _therules(l.table), _startrule(l.startrule),_context(c, l.expressions)
    ,_maxdepth(maxdepth), _minscale(minscale)
    {
        Dropgraphic::haveapt = false;
        for (auto & [name,rule] : _therules)
            rule.calculateParameters(_context);
        handlerule(_startrule, false, false, 1.0);
    }
    std::shared_ptr<Graphic> nextpoint();
    void draw();
    bool isDeepEnough();
private:
    void push(const Rule &rule, bool ruleRev, double flipFactor, double scaleBy);
    void pop();
    void graphic(const Motion &);
    void handlerule(const std::string &rr, bool rulerev, bool ruleflip, double localscale);

    Ruletable _therules;
    std::string _startrule;
    const Context _context;
    std::stack<Rulestate> _rulestates;
    unsigned int _maxdepth;
    Turtle _turtle;
    std::shared_ptr<Graphic> _agraphic;
    bool _backwards=false;
    double _minscale;
};

#endif
