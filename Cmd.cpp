#include "Cmd.h"
#include "Rulerunner.h"
#include "Turtle.h"

DrawStrategy & Command::artist(Rulerunner& rulerunner)
{
    return rulerunner.getDrawStrategy();
}

RotateCommand::RotateCommand(std::unique_ptr<Parsenode> _a) :
		_angleExpression(std::move(_a)) {
}

void RotateCommand::executeOn(Rulerunner& target, int /* unused */) {
	artist(target).rotate(_angle);
}

void RotateCommand::evaluateExpressions(const Context& context)
{
	_angle = _angleExpression->eval(context);
}

void FlipCommand::executeOn(Rulerunner& target, int /* unused */)
{
	artist(target).flip();
}

void PushCommand::executeOn(Rulerunner& target, int /* unused */)
{
	artist(target).push();
}

void PopCommand::executeOn(Rulerunner& target, int /* unused */)
{
	artist(target).pop();
}

RuleCommand::RuleCommand(std::string_view ruleName,
		bool isReversed, bool isFlipped, ParsenodePtr scaleExpression) :
		_ruleName(ruleName), _isReversed(isReversed), _isFlipped(isFlipped),
        _scaleExpression(std::move(scaleExpression)) {
}

void RuleCommand::executeOn(Rulerunner& target, int depth)
{
    const auto & rule = target.getRules().at(_ruleName);
    
    if (depth >= target.getMaxDepth()) {
        target.getDrawStrategy().draw(target,rule,_isReversed^_isFlipped,_atScale);
        return;
    }
    
    if (_isReversed ^ _isFlipped)
        artist(target).flip();
    auto oldScale = artist(target).getscale();
    artist(target).scaleby(_atScale*rule._localScale);
    target._backwards ^= _isReversed; //NOLINT

    if(target._backwards)
        for(auto i = rule._commands.rbegin(); i != rule._commands.rend(); ++i)
            (*i)->executeOn(target,depth+1);
    else
        for(auto &i : rule._commands)
            i->executeOn(target,depth+1);

    target._backwards ^= _isReversed; //NOLINT
    artist(target).setscale(oldScale);
    if (_isReversed ^ _isFlipped)
        artist(target).flip();
}

void RuleCommand::evaluateExpressions(const Context& context) {
	_atScale = _scaleExpression->eval(context);
}
