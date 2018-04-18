#include "Rulerunner.h"
using std::string;
#include "Cmd.h"

#include <memory>
#include <stdexcept>
using std::logic_error;
#include <cassert>

bool Rulerunner::isDeepEnough() {
    return _rulestates.size() > _maxdepth || _turtle.getscale() < _minscale;
}

void Rulerunner::push(const Rule &rule, bool ruleRev, double flipFactor, double scaleBy) {
    _backwards ^= ruleRev;
    _rulestates.push(Rulestate(&rule, _backwards, scaleBy, flipFactor));
    _turtle.scaleby(scaleBy);
    _turtle.flipBy(flipFactor);
}

void Rulerunner::pop() {
    _turtle.flipBy(_rulestates.top()._flipFactor);
    _turtle.scaleby(1.0/_rulestates.top().scaleBy);
    _rulestates.pop();
    _backwards = !_rulestates.empty() && _rulestates.top().backwards;
}

void Rulerunner::handlerule(const string &rr, bool rulerev, bool ruleflip, double atScale) {
    const auto & rule = _therules[rr];
    auto flipFactor = rulerev ^ ruleflip ? -1.0 : 1.0;
    if (isDeepEnough()) {
        _agraphic = _turtle.draw(rule,flipFactor,atScale);
    } else {
        push(rule,rulerev,flipFactor,atScale*rule._localScale);
    }
}

void Rulestate::doit(Rulerunner *towho) {//!!! Need to wrap rule so this can use reverse iterators
    if (backwards)
        (*--mypos)->execute(towho);
    else
        (*mypos++)->execute(towho);
}

std::shared_ptr<Graphic> Rulerunner::nextpoint() {
    if (_agraphic == nullptr) //!!! Should this be an assert?
        throw logic_error("Called nextpoint() on a Rulerunner with no graphic ready\n");
    std::shared_ptr<Graphic> temp(_agraphic);
    makeapoint();
    return temp;
}

void Rulerunner::makeapoint() {
    while (!_rulestates.empty() && _agraphic == nullptr) {
        if (_rulestates.top().done())
            pop();
        else
            _rulestates.top().doit(this);
    }
    if (_rulestates.empty()) {
        _finished = true;
        return;
    }
}

void Rulerunner::drawnextpoint() {
    assert(_agraphic != nullptr);
//        throw logic_error("Called drawnextpoint() on a Rulerunner with no graphic ready\n");
    _agraphic->draw();
    _agraphic.reset();
    makeapoint();
}
