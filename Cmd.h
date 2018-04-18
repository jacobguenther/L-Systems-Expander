#ifndef CMD_H
#define CMD_H

#include <string>
#include <list>
#include <utility>

#include "Context.h"
#include "Parser.h"
#include "Rulerunner.h"

class Command {
public:
    virtual void executeOn(Rulerunner *target) = 0;
    virtual void evaluateExpressions(const Context & /*unused*/) {}
    virtual ~Command() = default;
    Command() = default;
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;
    Command(Command&&) = delete;
    Command& operator=(Command&&) = delete;

private:
};

class RotateCommand : public Command {
public:
    explicit RotateCommand(std::shared_ptr<Parsenode> _a)
    : _angleExpression(std::move(_a))
    {}
    
    void executeOn(Rulerunner *target) override {
        target->_turtle.rotate(_angle);
    }
    
    void evaluateExpressions(const Context &cc) override {
        _angle = _angleExpression->eval(cc);
    }
    
private:
    std::shared_ptr<Parsenode> _angleExpression;
    double _angle;
};

class FlipCommand : public Command {
public:
    void executeOn(Rulerunner *target) override {
        target->_turtle.flip();
    }
};

class PushCommand : public Command {
public:
    void executeOn(Rulerunner *target) override { target->_turtle.push(); }
};

class PopCommand : public Command {
public:
    void executeOn(Rulerunner *target) override {
        target->_turtle.pop();
        Dropgraphic::haveapt = false;
    }
};

class RuleCommand : public Command {
public:
    explicit RuleCommand(std::string_view ruleName, bool isReversed , bool isFlipped ,
                     ParsenodePtr scaleExpression = Parser("1").parse())
    : _ruleName(ruleName)
    , _isReversed(isReversed)
    , _isFlipped(isFlipped)
    , _scaleExpression(std::move(scaleExpression))
    {}
    
    void executeOn(Rulerunner *target) override {
        target->handlerule(_ruleName, _isReversed, _isFlipped, _scale);
    }
    
    void evaluateExpressions(const Context &cc) override {
        _scale = _scaleExpression->eval(cc);
    }
    
private:
    const std::string _ruleName;
    bool _isReversed;
    bool _isFlipped;
    const ParsenodePtr _scaleExpression;
    double _scale;
};
#endif
