#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Rule.h"
#include "Turtle.h"
#include <string>

struct Rulerunner {
    friend class RuleCommand; //structstructnds need to modify _backwards
friend class Command; // all Commands modify our _drawStrategy turtle
public:
//    Rulerunner(const Ruletable &rules, DrawStrategy &drawStrategy);
    const Ruletable &_rules;
    DrawStrategy &_drawStrategy;
    bool _backwards=false;
};
#endif
