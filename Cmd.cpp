#include "Cmd.h"
#include "Rulerunner.h"

RotateCommand::RotateCommand(std::unique_ptr<Parsenode> _a) :
		_angleExpression(std::move(_a)) {
}

void RotateCommand::executeOn(Rulerunner& target) {
	target._turtle.rotate(_angle);
}

void RotateCommand::evaluateExpressions(const Context& context)
{
	_angle = _angleExpression->eval(context);
}

void FlipCommand::executeOn(Rulerunner& target)
{
	target._turtle.flip();
}

void PushCommand::executeOn(Rulerunner& target)
{
	target._turtle.push();
}

void PopCommand::executeOn(Rulerunner& target)
{
	target._turtle.pop();
	Dropgraphic::haveapt = false;
}

RuleCommand::RuleCommand(std::string_view ruleName,
		bool isReversed, bool isFlipped, ParsenodePtr scaleExpression) :
		_ruleName(ruleName), _isReversed(isReversed), _isFlipped(isFlipped), _scaleExpression(
				std::move(scaleExpression)) {
}

void RuleCommand::executeOn(Rulerunner& target)
{
	target.handlerule(_ruleName, _isReversed, _isFlipped, _scale);
}

void RuleCommand::evaluateExpressions(const Context& context) {
	_scale = _scaleExpression->eval(context);
}
