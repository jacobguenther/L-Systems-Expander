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
    if (ruleRunner._rulestates.top()._myRule.drawmethod==Rule::NONE)
        return;
    auto from = ruleRunner._turtle.getposition();
    ruleRunner._turtle.forward(1.0);
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
    Motion todraw;
//    todraw.frompt = storedpt;
//    double dx = m.topt.x - m.frompt.x;
//    double dy = m.topt.y - m.frompt.y;
//    storedpt.x = m.frompt.x + dd * (cos(da) * dx - sin(da) * dy);
//    storedpt.y = m.frompt.y + dd * (sin(da) * dx + cos(da) * dy);
//    todraw.topt = storedpt;
//    haveapt = true;
//    m = todraw;
}
