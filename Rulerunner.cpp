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

bool Rulerunner::isDeepEnough(int depth) {
    return depth >= _maxdepth || _turtle.getscale() < _minscale;
}

void Rulerunner::doCommand(Command &c,  bool reverse, bool flip, double scale, int depth) {
    if (reverse ^ flip)
        _turtle.flip();
    auto oldScale = _turtle.getscale();
    _turtle.scaleby(scale);
    _backwards ^= reverse; //NOLINT
    c.executeOn(*this,depth+1);
    _backwards ^= reverse; //NOLINT
    _turtle.setscale(oldScale);
    if (reverse ^ flip)
        _turtle.flip();
}

void Rulerunner::handlerule(const Rule &rule, bool rulerev, bool ruleflip, double atScale, int depth) {
    if (isDeepEnough(depth)) {
        _lSystem._drawStrategy->draw(*this,rule,ruleflip,atScale);
        return;
    }
    
    if(rulerev ^ _backwards)
        for(auto i = rule._commands.rbegin(); i != rule._commands.rend(); ++i)
            doCommand(**i,rulerev,ruleflip,atScale*rule._localScale,depth);
    else
        for(auto i = rule._commands.begin(); i != rule._commands.end(); ++i)
            doCommand(**i,rulerev,ruleflip,atScale*rule._localScale,depth);
}

void Rulerunner::draw() {
    _lSystem._drawStrategy->start();
    handlerule(_therules[_startrule], false, false, 1.0,0);
    _lSystem._drawStrategy->finish();
}

