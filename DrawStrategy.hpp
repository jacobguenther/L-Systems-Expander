//
//  DrawStrategy.hpp
//  lsystems
//
//  Created by Chris Hartman on 4/20/18.
//

#ifndef DrawStrategy_hpp
#define DrawStrategy_hpp

#include "Parsenode.h"
#include "Turtle.h"
#include <memory>
#include <vector>
class Rulerunner;

struct DrawStrategyToken {
    std::string _name;
    std::vector<std::string> _parameters;
};

class DrawStrategy {
public:
    virtual ~DrawStrategy() = default;
    DrawStrategy() = default;
    DrawStrategy(const DrawStrategy&) = delete;
    DrawStrategy& operator=(const DrawStrategy&) = delete;
    DrawStrategy(DrawStrategy&&) = delete;
    DrawStrategy& operator=(DrawStrategy&&) = delete;

    virtual void evaluateExpressions(const Context & /*context*/) const;
    void reset();
    void draw(const Rule &rule, bool ruleFlip, double atScale);
    void rotate(double angle);
    void flip();
    void push();
    void pop();
    void scaleby(double s);
    double getScale() const;
    void setScale(double s);
    Point getPosition() const;
    double getAngle() const;
    virtual void start();
    virtual void finish();
protected:
    const Turtle & turtle();
private:
    virtual void drawImpl(const Motion &m, bool ruleFlip);
    virtual void rotateImpl(double angle);
    virtual void flipImpl();
    virtual void pushImpl();
    virtual void popImpl();
    virtual void scalebyImpl(double s);
    virtual void setscaleImpl(double s);
    Turtle _turtle;
    double _theta=0.0;
};

using DrawStrategyPtr = std::unique_ptr<DrawStrategy>;

DrawStrategyPtr drawStrategyFactory(const DrawStrategyToken &dst);

class LinesDrawStrategy : public DrawStrategy {
public:
    void start() override;
    void finish() override;
private:
    void drawImpl(const Motion &m, bool ruleFlip) override;
};

class DropDrawStrategy : public DrawStrategy {
public:
    DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression);
    void evaluateExpressions(const Context &context) const override;
    void start() override;
    void finish() override;
private:
    void drawImpl(const Motion &m, bool ruleFlip) override;
    ParsenodePtr _dropAngleExpression;
    ParsenodePtr _dropDistanceExpression;
    mutable double _dropAngle=0.0;
    mutable double _dropDistance=0.5;
    Point _lastDropped;
    bool _hasDroppedPoint=false;
};

#endif /* DrawStrategy_hpp */
