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

void LinesDrawStrategy::draw(Rulerunner& ruleRunner) {
    if (ruleRunner._rulestates.top()._myRule.drawmethod == Rule::NONE)
        return;
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(1.0);
    if (ruleRunner._rulestates.top()._myRule.drawmethod==Rule::INVIS)
        return;
    auto to = ruleRunner._turtle.getposition();
    // !!! up to here is maybe a base class thing?
    glBegin(GL_LINES);
    glVertex2d(from.x, from.y);
    glVertex2d(to.x, to.y);
    glEnd();
}

DropDrawStrategy::DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression)
:_dropAngleExpression(move(dropAngleExpression)),_dropDistanceExpression(move(dropDistanceExpression))
{}

void DropDrawStrategy::draw(Rulerunner& ruleRunner) {
    if (ruleRunner._rulestates.top()._myRule.drawmethod == Rule::NONE){
        return;
    }
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(1.0);
    auto to = ruleRunner._turtle.getposition();
    glBegin(GL_LINES);
    glVertex2d(_lastDropped.x, _lastDropped.y);
    double dx = to.x - from.x;
    double dy = to.y - from.y;
    std::cout << from.x << "," << from.y << " to " << to.x << "," << to.y << "\n";
    std::cout << dx << " <-dx,dy-> " << dy << "\n";
    auto dd = _dropDistanceExpression->eval(ruleRunner._context);
    auto da = _dropAngleExpression->eval(ruleRunner._context)*ruleRunner._turtle.getflip()*DEG2RAD;//!!! cache these?
    std::cout << dd << " <-dd,da-> " << da << "\n";
    _lastDropped.x = from.x + dd * (cos(da) * dx - sin(da) * dy);
    _lastDropped.y = from.y + dd * (sin(da) * dx + cos(da) * dy);
    std::cout << _lastDropped.x << "," << _lastDropped.y << "\n";
    glVertex2d(_lastDropped.x, _lastDropped.y);
    glEnd();
}
