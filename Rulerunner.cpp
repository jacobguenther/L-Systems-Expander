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

const Context & Rulerunner::getContext() {
    return _context;
}

void Rulerunner::draw() {
    _lSystem._drawStrategy->start();
    handlerule(RuleInvocation{_therules[_startrule], false, false, 1.0,0});
    _lSystem._drawStrategy->finish();
}

bool Rulerunner::isDeepEnough(int depth) {
    return depth >= _maxdepth || _turtle.getscale() < _minscale;
}

void Rulerunner::doCommand(Command &c, const RuleInvocation &ri) {
    if (ri.rulerev ^ ri.ruleflip)
        _turtle.flip();
    auto oldScale = _turtle.getscale();
    _turtle.scaleby(ri.atScale*ri.rule._localScale);
    _backwards ^= ri.rulerev; //NOLINT
    c.executeOn(*this,ri.depth+1);
    _backwards ^= ri.rulerev; //NOLINT
    _turtle.setscale(oldScale);
    if (ri.rulerev ^ ri.ruleflip)
        _turtle.flip();
}

void Rulerunner::handlerule(const RuleInvocation &ri) {
    if (isDeepEnough(ri.depth)) {
        _lSystem._drawStrategy->draw(*this,ri.rule,ri.ruleflip,ri.atScale);
        return;
    }
    
    if(ri.rulerev ^ _backwards)
        for(auto i = ri.rule._commands.rbegin(); i != ri.rule._commands.rend(); ++i)
            doCommand(**i,ri);
    else
        for(auto i = ri.rule._commands.begin(); i != ri.rule._commands.end(); ++i)
            doCommand(**i,ri);
}

