#include "Rulerunner.h"
using std::string;
#include "Cmd.h"

#include <memory>
#include <stdexcept>
using std::logic_error;
#include <cassert>

bool Rulerunner::isDeepEnough() {
    return _rulestates.size() >= _maxdepth || _turtle.getscale() < _minscale;
}

void Rulerunner::push(const Rule &rule, bool ruleRev, double flipFactor, double scaleBy) {
    _backwards ^= ruleRev;
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

void Rulestate::runCurrentCommandOn(Rulerunner *toWho) {//!!! Need to wrap rule so this can use reverse iterators
    if (_isReversed)
        (*--_nextCommand)->execute(toWho);
    else
        (*_nextCommand++)->execute(toWho);
}

void Rulerunner::draw() {
    while (!_rulestates.empty()) {
        if (_rulestates.top().hasNoMoreCommands())
            pop();
        else
            _rulestates.top().runCurrentCommandOn(this);
    }
}
