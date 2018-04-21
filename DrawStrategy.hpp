//
//  DrawStrategy.hpp
//  lsystems
//
//  Created by Chris Hartman on 4/20/18.
//

#ifndef DrawStrategy_hpp
#define DrawStrategy_hpp

#include "Rule.h"
#include "Turtle.h"

class DrawStrategy {
public:
    virtual ~DrawStrategy() = default;
    DrawStrategy() = default;
    DrawStrategy(const DrawStrategy&) = delete;
    DrawStrategy& operator=(const DrawStrategy&) = delete;
    DrawStrategy(DrawStrategy&&) = delete;
    DrawStrategy& operator=(DrawStrategy&&) = delete;

    virtual void draw(Turtle &turtle, const Rule &rule, double flipFactor, double atScale) =0;
};

class LinesDrawStrategy : public DrawStrategy {
public:
    void draw(Turtle &turtle, const Rule &rule, double flipFactor, double atScale) override;
};
//!!! eventually might want rules to be able to start and stop drawing
//so an invisible rule will call start and stop so we know when to do glBegin etc.

class DropDrawStrategy : public DrawStrategy {
public:
    void draw(Turtle &turtle, const Rule &rule, double flipFactor, double atScale) override;
};





#endif /* DrawStrategy_hpp */
