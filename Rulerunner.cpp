// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rulerunner.h"
#include "Cmd.h"
#include <cassert>
#include <memory>

using std::string;

Rulerunner::Rulerunner(const Lsystem &l, int maxdepth, const Consttype &c,
                       DrawStrategyPtr drawStrategy)
    :_context(c, l.getExpressions())
    ,_maxdepth(maxdepth)
    , _lSystem(l)
    ,_drawStrategy(move(drawStrategy)) {
    for (auto & [name,rule] : _lSystem.getRules())
        rule.evaluateExpressions(_context); //!!! setContext should do ths
    getDrawStrategy().evaluateExpressions(_context);
}

const Context & Rulerunner::getContext() const {
    return _context;
}

void Rulerunner::draw(std::string_view startRule) {
    _drawStrategy->reset();
    _drawStrategy->start();
    RuleCommand(startRule, false, false).executeOn(*this, 0);
    _drawStrategy->finish();
}

int Rulerunner::getMaxDepth() const {
    return _maxdepth;
}

const Ruletable & Rulerunner::getRules() const {
    return _lSystem.getRules();
}

const DrawStrategy & Rulerunner::getDrawStrategy() const {
    return *_drawStrategy;
}
