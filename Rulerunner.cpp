// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rulerunner.h"
#include "Cmd.h"
#include <cassert>
#include <memory>

using std::string;

Rulerunner::Rulerunner(Lsystem &l, int maxdepth, double minscale, const Consttype &c)
: _context(c, l.expressions)
,_maxdepth(maxdepth), _minscale(minscale), _lSystem(l)
{
    for (auto & [name,rule] : _lSystem._rules)
        rule.calculateParameters(_context);
}

const Context & Rulerunner::getContext() const {
    return _context;
}

void Rulerunner::draw() {
    getDrawStrategy().reset();
    getDrawStrategy().start();
    RuleCommand(_lSystem.startrule, false, false).executeOn(*this, 0);
    getDrawStrategy().finish();
}

int Rulerunner::getMaxDepth() const {
    return _maxdepth;
}

const Ruletable & Rulerunner::getRules() const {
    return _lSystem._rules;
}

DrawStrategy & Rulerunner::getDrawStrategy() {
    return *_lSystem._drawStrategy;
}
