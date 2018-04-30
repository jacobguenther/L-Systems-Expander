#ifndef CMD_H
#define CMD_H

#include "Parser.h"
#include "Rulefwd.hpp"
#include <deque>
#include <memory>
#include <string>
#include <utility>

class DrawStrategy;

class Command {
protected:
    struct RunState {
        const Ruletable &_rules;
        DrawStrategy &_drawStrategy;
        bool _backwards=false;
    };
    
    void forwardExecuteTo(const Command &command, RunState &target, int depth) const;
    
private:
    virtual void executeOn(RunState &target, int depth) const = 0;

public:
    virtual ~Command() = default;
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;
    virtual void evaluateExpressions(const Context & /*unused*/) const;
};

using Commands = std::deque<std::unique_ptr<Command>>;

class RotateCommand : public Command {
public:
	explicit RotateCommand(std::unique_ptr<Parsenode> _a);
    void evaluateExpressions(const Context& context) const override;
    
private:
    void executeOn(RunState& target, int depth) const override;
    std::unique_ptr<Parsenode> _angleExpression;
    mutable double _angle=0.0;
};

class FlipCommand : public Command {
private:
	void executeOn(RunState& target, int depth) const override;
};

class PushCommand : public Command {
private:
    void executeOn(RunState& target, int depth) const override;
};

class PopCommand : public Command {
private:
    void executeOn(RunState& target, int depth) const override;
};

class RuleCommand : public Command {
public:
	explicit RuleCommand(std::string_view ruleName, bool isReversed,
			bool isFlipped, ParsenodePtr scaleExpression = parse("1.0"));
    
    static void run(std::string_view startrule, const Ruletable &_rules, DrawStrategy & drawStrategy, int depth);
    void evaluateExpressions(const Context& context)  const override;
    
private:
    void executeOn(RunState& target, int depth) const override;
    const std::string _ruleName;
    bool _isReversed;
    bool _isFlipped;
    const ParsenodePtr _scaleExpression;
    mutable double _atScale=1.0;
};
#endif
