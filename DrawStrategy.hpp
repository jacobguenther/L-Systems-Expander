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

class DrawStrategy {
    friend class Command;
public:
    virtual ~DrawStrategy() = default;
    DrawStrategy() = default;
    DrawStrategy(const DrawStrategy&) = delete;
    DrawStrategy& operator=(const DrawStrategy&) = delete;
    DrawStrategy(DrawStrategy&&) = delete;
    DrawStrategy& operator=(DrawStrategy&&) = delete;

    void reset();
    void draw(Rulerunner& ruleRunner, const Rule &rule, bool ruleFlip, double atScale);
    void rotate(double angle);
    void flip();
    void push();
    void pop();
    void scaleby(double s);
    double getscale() const;
    void setscale(double s);
    virtual void start();
    virtual void finish();
protected:
    const Turtle & turtle();
private:
    virtual void drawImpl(const Rulerunner& ruleRunner, const Motion &m, bool ruleFlip) =0;
    virtual void rotateImpl(double angle);
    virtual void flipImpl();
    virtual void pushImpl();
    virtual void popImpl();
    virtual void scalebyImpl(double s);
    virtual void setscaleImpl(double s);
    Turtle _turtle;
};

using DrawStrategyPtr = std::unique_ptr<DrawStrategy>;

DrawStrategyPtr drawStrategyFactory(std::string_view type, std::vector<ParsenodePtr> &&parameters = {});

class LinesDrawStrategy : public DrawStrategy {
public:
    void start() override;
    void finish() override;
private:
    void drawImpl(const Rulerunner& ruleRunner, const Motion &m, bool ruleFlip) override;
};
//!!! eventually might want rules to be able to start and stop drawing
//so an invisible rule will call start and stop so we know when to do glBegin etc.

class DropDrawStrategy : public DrawStrategy {
public:
    DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression);
    void start() override;
    void finish() override;
private:
    void drawImpl(const Rulerunner& ruleRunner, const Motion &m, bool ruleFlip) override;
    ParsenodePtr _dropAngleExpression;
    ParsenodePtr _dropDistanceExpression;
    Point _lastDropped;
    bool _hasDroppedPoint=false;
};

#endif /* DrawStrategy_hpp */
