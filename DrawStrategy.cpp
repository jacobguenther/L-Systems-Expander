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

void LinesDrawStrategy::draw(Turtle &turtle, const Rule &rule, double /*unused*/, double atScale) {
    if (rule.drawmethod==Rule::NONE)
        return;
    auto from = turtle.getposition();
    turtle.forward(atScale);
    auto to = turtle.getposition();
    // !!! up to here is maybe a base class thing?
    glBegin(GL_LINES);
    glVertex2d(from.x, from.y);
    glVertex2d(to.x, to.y);
    glEnd();
}

DropDrawStrategy::DropDrawStrategy(ParsenodePtr dropAngleExpression, ParsenodePtr dropDistanceExpression)
:_dropAngleExpression(move(dropAngleExpression)),_dropDistanceExpression(move(dropDistanceExpression))
{}

void DropDrawStrategy::draw(Turtle &turtle, const Rule &rule, double flipFactor, double atScale) {
    
}
