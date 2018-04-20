// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Rulerunner.h"
using std::string;
#include "Cmd.h"

#include <memory>
#include <cassert>

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
    const auto & rule = _therules[rr];
    auto flipFactor = rulerev ^ ruleflip ? -1.0 : 1.0;
    if (isDeepEnough()) {
        _turtle.draw(rule,flipFactor,atScale)->draw(); //!!!Turtle should just draw
    } else {
        push(rule,rulerev,flipFactor,atScale*rule._localScale);
    }
}

void Rulestate::runCurrentCommandOn(Rulerunner &target) {
    if (_isReversed)
        (*--_nextCommand)->executeOn(target);
    else
        (*_nextCommand++)->executeOn(target);
}

void Rulerunner::draw() {
    while (!_rulestates.empty()) {
        if (_rulestates.top().hasNoMoreCommands())
            pop();
        else
            _rulestates.top().runCurrentCommandOn(*this);
    }
}
