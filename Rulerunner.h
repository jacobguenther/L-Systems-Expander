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
    Rulerunner(const Ruletable &rules, const Context &context,
               DrawStrategyPtr drawStrategy);
    const DrawStrategy & getDrawStrategy() const;
    void draw(std::string_view startRule, int level);
    int getMaxDepth() const;
private:
    bool _backwards=false;
    const Ruletable &_rules;
    DrawStrategyPtr _drawStrategy;
};
#endif
