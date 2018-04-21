#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Graphic.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Rule.h"
#include "RuleState.h"
#include "Turtle.h"
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>


class Lexer;

//class Rulestate
//Member variables are
//   bool _isReversed = is the currently running rule reversed
//   double _flipFactor = do we flip the turtle when we start running this rule
//   const Rule & _myRule  = the currently running rule
//   Cmdcont::const_iterator _nextCommand = the next command to run
//   double _scaleFactor = what did we scale by when starting this rule?

class Rulerunner {
friend class RotateCommand;  //!!!need friends, or make a public turtle accessor?
friend class FlipCommand;
friend class PopCommand;
friend class PushCommand;
friend class RuleCommand;
public:
    Rulerunner(Lsystem &l, unsigned int maxdepth, double minscale, const Consttype &c);
    std::shared_ptr<Graphic> nextpoint();
    void draw();
    bool isDeepEnough();
private:
    void push(const Rule &rule, bool ruleRev, double flipFactor, double scaleBy);
    void pop();
    void graphic(const Motion &);
    void handlerule(const std::string &rr, bool rulerev, bool ruleflip, double atScale);

    Ruletable &_therules; //Would like to be const, but calculating/caching parameters changes
    std::string _startrule;
    const Context _context;
    std::stack<Rulestate> _rulestates;
    unsigned int _maxdepth;
    Turtle _turtle;
    std::shared_ptr<Graphic> _agraphic;
    bool _backwards=false;
    double _minscale;
    Lsystem &_lSystem; //Think about this. Do we really want parent pointers?!!!
};
#endif
