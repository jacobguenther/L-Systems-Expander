// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rulerunner.h"
#include "Cmd.h"
#include <cassert>
#include <memory>

using std::string;

Rulerunner::Rulerunner(const Ruletable &rules, const Context &context,
                       DrawStrategyPtr drawStrategy)
    :_rules(rules)
    ,_drawStrategy(move(drawStrategy)) {
    for (auto & [name,rule] : _rules)
        rule.evaluateExpressions(context); //!!! setContext should do ths
    getDrawStrategy().evaluateExpressions(context);
}

void Rulerunner::draw(std::string_view startRule, int level) {
    _drawStrategy->reset();
    _drawStrategy->start();
    RuleCommand(startRule, false, false).executeOn(*this, level);
    _drawStrategy->finish();
}

const DrawStrategy & Rulerunner::getDrawStrategy() const {
    return *_drawStrategy;
}
