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

using std::move;
using std::make_unique;

DrawStrategyPtr drawStrategyFactory(std::string_view type, std::vector<ParsenodePtr> &&parameters) {
    if (type == "drop")
        return make_unique<DropDrawStrategy>(move(parameters[0]),move(parameters[1]));
    if (type == "normal")
        return make_unique<LinesDrawStrategy>();
    throw std::runtime_error("Unrecognized draw strategy in drawStrategyFactory");
}

void DrawStrategy::draw(Rulerunner& ruleRunner, const Rule &rule, bool ruleFlip, double atScale) {
    if (rule._doesNotDraw)
        return;
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(atScale);
    if (rule._drawsInvisibly)
        return;
    auto to = ruleRunner._turtle.getposition();
    drawImpl(ruleRunner, {from,to}, ruleFlip);
}

void LinesDrawStrategy::drawImpl(const Rulerunner& /*ruleRunner*/, const Motion &m, bool /*ruleFlip*/) {
    glVertex2d(m.frompt.x, m.frompt.y);
    glVertex2d(m.topt.x, m.topt.y);
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

void DropDrawStrategy::drawImpl(const Rulerunner& ruleRunner, const Motion &m, bool ruleFlip) {
    double dx = m.topt.x - m.frompt.x;
    double dy = m.topt.y - m.frompt.y;
    auto dd = _dropDistanceExpression->eval(ruleRunner.getContext());
    auto da = _dropAngleExpression->eval(ruleRunner.getContext())*ruleRunner._turtle.getflip()*DEG2RAD;
    //!!! cache these, then won't need to pass in the rulerunner
    if (ruleFlip)
        da *=-1;
    auto nextX = m.frompt.x + dd * (cos(da) * dx - sin(da) * dy);
    auto nextY = m.frompt.y + dd * (sin(da) * dx + cos(da) * dy);
    
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
