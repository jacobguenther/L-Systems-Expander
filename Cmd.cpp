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
    target.handlerule(Rulerunner::RuleInvocation{target._therules[_ruleName], _isReversed, _isFlipped, _scale, depth});
}

void RuleCommand::evaluateExpressions(const Context& context) {
	_scale = _scaleExpression->eval(context);
}
