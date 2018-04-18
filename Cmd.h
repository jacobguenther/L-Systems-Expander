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
    virtual void execute(Rulerunner *ruleRunner) = 0;
    virtual ~Command()=default;
    virtual void evaluateExpressions(const Context & /*unused*/) {}
    
private:
};

class RotateCommand : public Command {
public:
    explicit RotateCommand(std::shared_ptr<Parsenode> _a)
    : angle(std::move(_a))
    {}
    
    void execute(Rulerunner *ruleRunner) override {
        ruleRunner->_turtle.rotate(cachedangle);
    }
    
    void evaluateExpressions(const Context &cc) override {
        cachedangle = angle->eval(cc);
    }
    
private:
    std::shared_ptr<Parsenode> angle;
    double cachedangle;
};

class FlipCommand : public Command {
public:
    void execute(Rulerunner *ruleRunner) override {
        ruleRunner->_turtle.flip();
    }
};

class PushCommand : public Command {
public:
    void execute(Rulerunner *ruleRunner) override { ruleRunner->_turtle.push(); }
};

class PopCommand : public Command {
public:
    void execute(Rulerunner *ruleRunner) override {
        ruleRunner->_turtle.pop();
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
    
    void execute(Rulerunner *ruleRunner) override {
        ruleRunner->handlerule(_ruleName, _isReversed, _isFlipped, _scale);
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
