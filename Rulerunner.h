#ifndef RULERUNNER_H
#define RULERUNNER_H

#include "Context.h"
#include "Lsystem.h"
#include "Parser.h"
#include "Rule.h"
#include "Turtle.h"
#include <list>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Lexer;

class Rulerunner {
friend class RuleCommand; // RuleCommands need to modify _backwards
friend class Command; // all Commands modify our _drawStrategy turtle
public:
    Rulerunner(const Lsystem &l, int maxdepth, const Consttype &c,
               DrawStrategyPtr drawStrategy);
    const Context & getContext() const;
    const DrawStrategy & getDrawStrategy() const;
    void draw(std::string_view startRule);
    int getMaxDepth() const;
    const Ruletable & getRules() const;
private:
    const Context _context;
    int _maxdepth;
    bool _backwards=false;
    const Lsystem &_lSystem;
    DrawStrategyPtr _drawStrategy;
};
#endif
