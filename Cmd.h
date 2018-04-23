#ifndef CMD_H
#define CMD_H

#include "Parser.h"
#include <list>
#include <string>
#include <utility>

class Context;
class DrawStrategy;
class Rulerunner;
class Turtle;

class Command {
protected:
    DrawStrategy & artist(Rulerunner& rulerunner);
public:
    virtual ~Command() = default;
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;
    virtual void executeOn(Rulerunner &target, int depth) = 0;
    virtual void evaluateExpressions(const Context & /*unused*/) {}
};

using Commands = std::vector<std::unique_ptr<Command>>;

class RotateCommand : public Command {
public:
	explicit RotateCommand(std::unique_ptr<Parsenode> _a);
    void executeOn(Rulerunner& target, int depth) override;
    void evaluateExpressions(const Context& context) override;
    
private:
    std::unique_ptr<Parsenode> _angleExpression;
    double _angle=0.0;
};

class FlipCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) override;
};

class PushCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) override;
};

class PopCommand : public Command {
public:
	void executeOn(Rulerunner& target, int depth) override;
};

class RuleCommand : public Command {
public:
	explicit RuleCommand(std::string_view ruleName, bool isReversed,
			bool isFlipped, ParsenodePtr scaleExpression = Parser("1.0").parse());
    
    void executeOn(Rulerunner& target, int depth) override;
    
    void evaluateExpressions(const Context& context) override;
    
private:
    const std::string _ruleName;
    bool _isReversed;
    bool _isFlipped;
    const ParsenodePtr _scaleExpression;
    double _atScale=1.0;
};
#endif
