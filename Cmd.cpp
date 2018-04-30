#include "Cmd.h"
#include "DrawStrategy.hpp"
#include "Rule.h"
#include "Turtle.h"

void Command::evaluateExpressions(const Context& /*context*/) const
{}

void Command::forwardExecuteTo(const Command &command, RunState &target, int depth) const {
    command.executeOn(target, depth);
}

RotateCommand::RotateCommand(std::unique_ptr<Parsenode> _a) :
		_angleExpression(std::move(_a)) {
}

void RotateCommand::executeOn(RunState& target, int /* unused */) const {
	target._drawStrategy.rotate(_angle);
}

void RotateCommand::evaluateExpressions(const Context& context) const{
	_angle = _angleExpression->eval(context);
}

void FlipCommand::executeOn(RunState& target, int /* unused */) const
{
	target._drawStrategy.flip();
}

void PushCommand::executeOn(RunState& target, int /* unused */) const
{
	target._drawStrategy.push();
}

void PopCommand::executeOn(RunState& target, int /* unused */) const
{
	target._drawStrategy.pop();
}

RuleCommand::RuleCommand(std::string_view ruleName,
		bool isReversed, bool isFlipped, ParsenodePtr scaleExpression) :
		_ruleName(ruleName), _isReversed(isReversed), _isFlipped(isFlipped),
        _scaleExpression(std::move(scaleExpression)) {
}

void RuleCommand::run(std::string_view startrule,const Ruletable &_rules, DrawStrategy & drawStrategy, int depth) {
    RunState rr{_rules,drawStrategy};
    RuleCommand(startrule,false,false).executeOn(rr , depth);
}

template <typename Func>
void forwardOrBackwards(bool backwards,const Commands &c, Func f)
{
    if(backwards)
        for(auto i = c.rbegin(); i != c.rend(); ++i)
            f(i);
    else
        for(auto i = c.begin(); i != c.end(); ++i)
            f(i);
}

void RuleCommand::executeOn(RunState& target, int depth) const
{
    const auto & rule = target._rules.at(_ruleName);
    
    if (depth == 0) {
        target._drawStrategy.draw(rule,_isFlipped,_atScale);
        return;
    }
    
    auto oldScale = target._drawStrategy.getScale();
    target._drawStrategy.scaleby(_atScale*rule.getLocalScale());
    if (_isFlipped)
        target._drawStrategy.flip();
    target._backwards ^= _isReversed; //NOLINT

    forwardOrBackwards(target._backwards, rule.getCommands(), [&](auto i){
        forwardExecuteTo(**i,target,depth-1);});

    target._backwards ^= _isReversed; //NOLINT
    if (_isFlipped)
        target._drawStrategy.flip();
    target._drawStrategy.setScale(oldScale);
}

void RuleCommand::evaluateExpressions(const Context& context) const {
	_atScale = _scaleExpression->eval(context);
}
