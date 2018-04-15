#include "Rulerunner.h"
#include "Cmd.h"

#include <memory>
#include <stdexcept>
using std::logic_error;

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

void Rulerunner::handlerule(const std::string &rr, bool rulerev, bool ruleflip,
                            double localscale) {
    bool willflip = rulerev ^ ruleflip;
    if (_rulestates.size() >= _maxdepth || localscale * _turtle.getscale() < _minscale) {
        Motion temp;
        temp.frompt = _turtle.getposition();
        _turtle.forward(localscale);  //!!!  Here is where we fix issue #5
        temp.topt = _turtle.getposition();
        switch (_therules[rr].drawmethod) {  //!!!Use a factory here!
            case Rule::NORM:
                _agraphic = std::make_shared<Linegraphic>(temp);
                break;
            case Rule::MIDPT:
                _agraphic = std::make_shared<Dropgraphic>(temp, 0, .5);
                break;
            case Rule::INVIS:
                _agraphic = std::make_shared<Invisgraphic>();
                break;
            case Rule::RECT:
                _agraphic = std::make_shared<Linegraphic>(temp);
                break;  //!!!
            case Rule::DROP:
                double ffac = (willflip ? -1 : 1) * _turtle.getflip();
                _agraphic = std::make_shared<Dropgraphic>(temp,
                                                          ffac * _therules[rr].cacheddropangle,
                                                          _therules[rr].cacheddropdistance);
                break;  //!!!
        }
    } else {
        bool currentlybw = _rulestates.empty() ? false : _rulestates.top().backwards;
        _rulestates.push(Rulestate(&_therules[rr], currentlybw ^ rulerev,
                                  _turtle.getscale(), willflip));
        double newscalefac = localscale * _therules[rr].cachedscalefac; //localscale is A@ 2 notation, cachedscalefac is A ? localscale 1/sqrt(2) notation
        _turtle.scaleby(newscalefac);
        if (willflip) _turtle.flip();
    }
}

void Rulestate::doit(Rulerunner *towho) {
    if (backwards)
        (*--mypos)->execute(towho);
    else
        (*mypos++)->execute(towho);
}

std::shared_ptr<Graphic> Rulerunner::nextpoint() {
    if (_agraphic == nullptr)
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
    if (agraphic.get() == 0)
        throw logic_error("Called drawnextpoint() on a Rulerunner with no graphic ready\n");
#endif
    _agraphic->draw();
    _agraphic.reset();
    makeapoint();
}
