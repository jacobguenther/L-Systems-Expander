//
//  DrawStrategy.cpp
//  lsystems
//
//  Created by Chris Hartman on 4/20/18.
//

#include "DrawStrategy.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Rulerunner.h"

void LinesDrawStrategy::draw(Rulerunner& ruleRunner, Rule &rule, bool /*ruleFlip*/, double atScale) {
    if (rule._doesNotDraw)
        return;
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(atScale);
    if (rule._drawsInvisibly)
        return;
    auto to = ruleRunner._turtle.getposition();
    // !!! up to here is maybe a base class thing? Use Sutter's Non-Virtual Interface Idiom!
    glVertex2d(from.x, from.y);
    glVertex2d(to.x, to.y);
}

void LinesDrawStrategy::start() {
    glBegin(GL_LINES);
}

void LinesDrawStrategy::finish() {
    glEnd();
}

DropDrawStrategy::DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression)
:_dropAngleExpression(move(dropAngleExpression)),_dropDistanceExpression(move(dropDistanceExpression))
{}

void DropDrawStrategy::draw(Rulerunner& ruleRunner, Rule &rule, bool ruleFlip, double atScale) {
    if (rule._doesNotDraw)
        return;
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(atScale);
    if (rule._drawsInvisibly)
        return;
    auto to = ruleRunner._turtle.getposition();
    
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    auto dd = _dropDistanceExpression->eval(ruleRunner._context);
    auto da = _dropAngleExpression->eval(ruleRunner._context)*ruleRunner._turtle.getflip()*DEG2RAD;//!!! cache these?
    if (ruleFlip)
        da *=-1;
    auto nextX = from.x + dd * (cos(da) * dx - sin(da) * dy);
    auto nextY = from.y + dd * (sin(da) * dx + cos(da) * dy);
    
    if(!_hasDroppedPoint) {
        _lastDropped = {nextX,nextY};
        _hasDroppedPoint = true;
        return;
    }

    glVertex2d(_lastDropped.x, _lastDropped.y);
    glVertex2d(nextX, nextY);

    _lastDropped = {nextX,nextY};
}

void DropDrawStrategy::start() {
    glBegin(GL_LINES);
}

void DropDrawStrategy::finish() {
    glEnd();
    _hasDroppedPoint = false;
}
