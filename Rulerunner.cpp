#include "Rulerunner.h"
using std::string;
#include "Cmd.h"

#include <memory>
#include <stdexcept>
using std::logic_error;


#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

void Rulerunner::handlerule(const string &rr, bool rulerev, bool ruleflip, double localscale) {
    const auto & rule = _therules[rr]; //!!!eventually decorate with reverse and maybe flip?
    if (_rulestates.size() >= _maxdepth || _turtle.getscale() < _minscale) {
        if(rulerev ^ ruleflip)
            _turtle.flip();
        _turtle.scaleby(localscale);
        _agraphic = _turtle.draw(rule);
        _turtle.scaleby(1.0/localscale);
        if(rulerev ^ ruleflip)
            _turtle.flip();
    } else {
        bool willflip = rulerev ^ ruleflip;
        bool currentlybw = _rulestates.empty() ? false : _rulestates.top().backwards; //!!! Should start with start rule on stack, then not check for empty() here
        _rulestates.push(Rulestate(&rule, currentlybw ^ rulerev, _turtle.getscale(), willflip));
        if (willflip)
            _turtle.flip();
        _turtle.scaleby(localscale * rule.cachedscalefac);//localscale is A@ 2 notation, cachedscalefac is A ? localscale 1/sqrt(2) notation
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
    while (!_rulestates.empty() && !_agraphic.get()) {  //Go until we hit a turtle forward, or we're done
        
        if (_rulestates.top().done()) {
            _turtle.setscale(_rulestates.top().oldscale);
            if (_rulestates.top().flipped)
                _turtle.flip();
            _rulestates.pop();
        } else
            _rulestates.top().doit(this);
    }
    if (_rulestates.empty() && !_agraphic.get())
        _finished = true;
}

void Rulerunner::drawnextpoint() {
#ifdef _DEBUG
    if (_agraphic == nullptr)
        throw logic_error("Called drawnextpoint() on a Rulerunner with no graphic ready\n");
#endif
    _agraphic->draw();
    _agraphic.reset();
    makeapoint();
}
