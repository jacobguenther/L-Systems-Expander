// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rulerunner.h"
#include "Cmd.h"
#include <cassert>
#include <memory>

using std::string;

Rulerunner::Rulerunner(Lsystem &l, unsigned int maxdepth, double minscale, const Consttype &c)
: _therules(l.table), _startrule(l.startrule),_context(c, l.expressions)
,_maxdepth(maxdepth), _minscale(minscale), _lSystem(l)
{
    Dropgraphic::haveapt = false;
    for (auto & [name,rule] : _therules)
        rule.calculateParameters(_context);
    handlerule(_startrule, false, false, 1.0);
}

bool Rulerunner::isDeepEnough() {
    return _rulestates.size() >= _maxdepth || _turtle.getscale() < _minscale;
}

void Rulerunner::push(const Rule &rule, bool ruleRev, double flipFactor, double scaleBy) {
    _backwards ^= ruleRev; //NOLINT
    _rulestates.push(Rulestate(rule, _backwards, scaleBy, flipFactor));
    _turtle.scaleby(scaleBy);
    _turtle.flipBy(flipFactor);
}

void Rulerunner::pop() {
    _turtle.flipBy(_rulestates.top()._flipFactor);
    _turtle.scaleby(1.0/_rulestates.top()._scaleFactor);
    _rulestates.pop();
    _backwards = !_rulestates.empty() && _rulestates.top()._isReversed;
}

void Rulerunner::handlerule(const string &rr, bool rulerev, bool ruleflip, double atScale) {
    const auto & rule = _therules.at(rr);
    auto flipFactor = rulerev ^ ruleflip ? -1.0 : 1.0;
    if (isDeepEnough())
        _lSystem._drawStrategy->draw(_turtle,rule,flipFactor,atScale);
    else
        push(rule,rulerev,flipFactor,atScale*rule._localScale);
}

