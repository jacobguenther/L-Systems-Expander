#include "Cmd.h"
#include "Rulerunner.h"
#include "Turtle.h"

Turtle & Command::turtle(Rulerunner& rulerunner)
{
    return rulerunner._turtle;
}

RotateCommand::RotateCommand(std::unique_ptr<Parsenode> _a) :
		_angleExpression(std::move(_a)) {
}

void RotateCommand::executeOn(Rulerunner& target, int /* unused */) {
	turtle(target).rotate(_angle);
}

void RotateCommand::evaluateExpressions(const Context& context)
{
	_angle = _angleExpression->eval(context);
}

void FlipCommand::executeOn(Rulerunner& target, int /* unused */)
{
	turtle(target).flip();
}

void PushCommand::executeOn(Rulerunner& target, int /* unused */)
{
	turtle(target).push();
}

void PopCommand::executeOn(Rulerunner& target, int /* unused */)
{
	turtle(target).pop();
    target._lSystem._drawStrategy->finish();
    target._lSystem._drawStrategy->start();
}

RuleCommand::RuleCommand(std::string_view ruleName,
		bool isReversed, bool isFlipped, ParsenodePtr scaleExpression) :
		_ruleName(ruleName), _isReversed(isReversed), _isFlipped(isFlipped),
        _scaleExpression(std::move(scaleExpression)) {
}

void RuleCommand::executeOn(Rulerunner& target, int depth)
{
    auto & rule = target._therules[_ruleName];
    
    if (target.isDeepEnough(depth)) {
        target._lSystem._drawStrategy->draw(target,rule,_isReversed^_isFlipped,_atScale);
        return;
    }
    
    if (_isReversed ^ _isFlipped)
        turtle(target).flip();
    auto oldScale = turtle(target).getscale();
    turtle(target).scaleby(_atScale*rule._localScale);
    target._backwards ^= _isReversed; //NOLINT

    if(target._backwards)
        for(auto i = rule._commands.rbegin(); i != rule._commands.rend(); ++i)
            (*i)->executeOn(target,depth+1);
    else
        for(auto &i : rule._commands)
            i->executeOn(target,depth+1);

    target._backwards ^= _isReversed; //NOLINT
    turtle(target).setscale(oldScale);
    if (_isReversed ^ _isFlipped)
        turtle(target).flip();
}

void RuleCommand::evaluateExpressions(const Context& context) {
	_atScale = _scaleExpression->eval(context);
}
