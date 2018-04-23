// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Rulerunner.h"
#include "Cmd.h"
#include <cassert>
#include <memory>

using std::string;

Rulerunner::Rulerunner(Lsystem &l, int maxdepth, double minscale, const Consttype &c)
: _therules(l.table), _startrule(l.startrule),_context(c, l.expressions)
,_maxdepth(maxdepth), _minscale(minscale), _lSystem(l)
{
    for (auto & [name,rule] : _therules)
        rule.calculateParameters(_context);
//    handlerule(_therules[_startrule], false, false, 1.0); //!!!
}

const Context & Rulerunner::getContext() const {
    return _context;
}

void Rulerunner::draw() {
    _lSystem._drawStrategy->start();
    RuleCommand(_startrule, false, false).executeOn(*this, 0);
    _lSystem._drawStrategy->finish();
}

bool Rulerunner::isDeepEnough(int depth) {
    return depth >= _maxdepth || _turtle.getscale() < _minscale;
}

