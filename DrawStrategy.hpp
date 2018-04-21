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

class Rulerunner;

class DrawStrategy {
public:
    virtual ~DrawStrategy() = default;
    DrawStrategy() = default;
    DrawStrategy(const DrawStrategy&) = delete;
    DrawStrategy& operator=(const DrawStrategy&) = delete;
    DrawStrategy(DrawStrategy&&) = delete;
    DrawStrategy& operator=(DrawStrategy&&) = delete;

    virtual void draw(Rulerunner& ruleRunner) =0;
};

using DrawStrategyPtr = std::unique_ptr<DrawStrategy>;

class LinesDrawStrategy : public DrawStrategy {
public:
    void draw(Rulerunner& ruleRunner) override;
};
//!!! eventually might want rules to be able to start and stop drawing
//so an invisible rule will call start and stop so we know when to do glBegin etc.

class DropDrawStrategy : public DrawStrategy {
public:
    DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression);
    void draw(Rulerunner& ruleRunner) override;
private:
    ParsenodePtr _dropAngleExpression;
    ParsenodePtr _dropDistanceExpression;
};





#endif /* DrawStrategy_hpp */
