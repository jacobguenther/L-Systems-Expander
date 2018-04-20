#ifndef CMD_H
#define CMD_H

#include "Context.h"
#include "Parser.h"
#include "Rulerunner.h"
#include <list>
#include <string>
#include <utility>

class Command {
public:
    virtual ~Command() = default;
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;
    virtual void executeOn(Rulerunner &target) = 0;
    virtual void evaluateExpressions(const Context & /*unused*/) {}
};

class RotateCommand : public Command {
public:
	explicit RotateCommand(std::shared_ptr<Parsenode> _a);
    void executeOn(Rulerunner& target) override;
    void evaluateExpressions(const Context& context) override;
    
private:
    std::shared_ptr<Parsenode> _angleExpression;
    double _angle=0.0;
};

class FlipCommand : public Command {
public:
	void executeOn(Rulerunner& target) override;
};

class PushCommand : public Command {
public:
	void executeOn(Rulerunner& target) override;
};

class PopCommand : public Command {
public:
	void executeOn(Rulerunner& target) override;
};

class RuleCommand : public Command {
public:
	explicit RuleCommand(std::string_view ruleName, bool isReversed,
			bool isFlipped, ParsenodePtr scaleExpression = Parser("1").parse());
    
    void executeOn(Rulerunner& target) override;
    
    void evaluateExpressions(const Context& context) override;
    
private:
    const std::string _ruleName;
    bool _isReversed;
    bool _isFlipped;
    const ParsenodePtr _scaleExpression;
    double _scale=1.0;
};
#endif
