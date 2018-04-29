#ifndef CMD_H
#define CMD_H

#include "Parser.h"
#include <list>
#include <string>
#include <utility>
#include <deque>

class Context;
class DrawStrategy;
class Rulerunner;
class Turtle;

class Command {
protected:
    DrawStrategy & artist(const Rulerunner& rulerunner) const;
public:
    virtual ~Command() = default;
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;
    virtual void executeOn(Rulerunner &target, int depth) const = 0;
    virtual void evaluateExpressions(const Context & /*unused*/) const;
};

using Commands = std::deque<std::unique_ptr<Command>>;

class RotateCommand : public Command {
public:
	explicit RotateCommand(std::unique_ptr<Parsenode> _a);
    void executeOn(Rulerunner& target, int depth) const override;
    void evaluateExpressions(const Context& context) const override;
    
private:
    std::unique_ptr<Parsenode> _angleExpression;
    mutable double _angle=0.0;
};

class FlipCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) const override;
};

class PushCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) const override;
};

class PopCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) const override;
};

class RuleCommand : public Command {
public:
	explicit RuleCommand(std::string_view ruleName, bool isReversed,
			bool isFlipped, ParsenodePtr scaleExpression = parse("1.0"));
    
    void executeOn(Rulerunner& target, int depth) const override;
    
    void evaluateExpressions(const Context& context)  const override;
    
private:
    const std::string _ruleName;
    bool _isReversed;
    bool _isFlipped;
    const ParsenodePtr _scaleExpression;
    mutable double _atScale=1.0;
};
#endif
