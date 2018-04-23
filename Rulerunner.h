#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Rule.h"
#include "Turtle.h"
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <string>

class Lexer;

class Rulerunner {
friend class RuleCommand; //Rules need to modify _backwards
public:
    Rulerunner(const Lsystem &l, int maxdepth, double minscale, const Consttype &c);
    const Context & getContext() const;
    DrawStrategy & getDrawStrategy() const;
    void draw();
    int getMaxDepth() const;
    const Ruletable & getRules() const;
private:
    const Context _context;
    int _maxdepth;
    bool _backwards=false;
    double _minscale;
    const Lsystem &_lSystem; 
};
#endif
