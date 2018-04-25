//
//  DrawStrategy.cpp
//  lsystems
//
//  Created by Chris Hartman on 4/20/18.
//

#include "Color.hpp"
#include "DrawStrategy.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "Rulerunner.h"

using std::move;
using std::make_unique;

DrawStrategyPtr drawStrategyFactory(const DrawStrategyToken &dst) {
    if (dst._name == "drop") {
        auto angle = parse(dst._parameters[0]);
        auto distance = parse(dst._parameters[1]);
        return make_unique<DropDrawStrategy>(move(angle),move(distance));
    }
    if (dst._name == "normal")
        return make_unique<LinesDrawStrategy>();
    throw std::runtime_error("Unrecognized draw strategy in drawStrategyFactory");
}

void DrawStrategy::evaluateExpressions(const Context& /*context*/) const
{}

void DrawStrategy::reset() {
    _turtle = Turtle{};
}

void DrawStrategy::draw(const Rule &rule, bool ruleFlip, double atScale) {
    if (rule.doesNotDraw())
        return;
    auto from = _turtle.getposition();
    _turtle.forward(atScale);
    if (rule.drawsInvisibly())
        return;
    auto to = _turtle.getposition();
    drawImpl({from,to}, ruleFlip);
    _theta += 2*M_PI*((from.x-to.x)*(from.x-to.x)+(from.y-to.y)*(from.y-to.y));
//    std::cout << _theta << "\n";
    Color::colorCircle(_theta).glSet();
}

void DrawStrategy::rotate(double angle){
    _turtle.rotate(angle);
    rotateImpl(angle);
}

void DrawStrategy::flip(){
    _turtle.flip();
    flipImpl();
}

void DrawStrategy::push(){
    _turtle.push();
    pushImpl();
}

void DrawStrategy::pop(){
    _turtle.pop();
    finish();
    start();
    popImpl();
}

void DrawStrategy::scaleby(double s){
    _turtle.scaleby(s);
    scalebyImpl(s);
}

double DrawStrategy::getscale() const {
    return _turtle.getscale();
}

void DrawStrategy::setscale(double s){
    _turtle.setscale(s);
    setscaleImpl(s);
}

void DrawStrategy::scalebyImpl(double /*s*/){
}

void DrawStrategy::setscaleImpl(double /*s*/){
}

void DrawStrategy::rotateImpl(double /*angle*/){
}

void DrawStrategy::flipImpl(){
}

void DrawStrategy::pushImpl(){
}

void DrawStrategy::popImpl(){
}

void DrawStrategy::start(){}

void DrawStrategy::finish(){}

const Turtle & DrawStrategy::turtle() {
    return _turtle;
}

void LinesDrawStrategy::drawImpl(const Motion &m, bool /*ruleFlip*/) {
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
:_dropAngleExpression(move(dropAngleExpression)),_dropDistanceExpression(move(dropDistanceExpression)) {}

void DropDrawStrategy::evaluateExpressions(const Context& context) const {
    _dropAngle = _dropAngleExpression->eval(context);
    _dropDistance = _dropDistanceExpression->eval(context);
}

void DropDrawStrategy::start() {
    _hasDroppedPoint = false;
    glBegin(GL_LINES);
}

void DropDrawStrategy::finish() {
    glEnd();
}

void DropDrawStrategy::drawImpl(const Motion &m, bool ruleFlip) {
    double dx = m.topt.x - m.frompt.x;
    double dy = m.topt.y - m.frompt.y;
    //!!! cache these, then won't need to pass in the rulerunner
    auto da = _dropAngle*turtle().getflip()*DEG2RAD;
    if (ruleFlip)
        da *=-1;
    auto nextX = m.frompt.x + _dropDistance * (cos(da) * dx - sin(da) * dy);
    auto nextY = m.frompt.y + _dropDistance * (sin(da) * dx + cos(da) * dy);
    
    if(!_hasDroppedPoint) {
        _lastDropped = {nextX,nextY};
        _hasDroppedPoint = true;
        return;
    }

    glVertex2d(_lastDropped.x, _lastDropped.y);
    glVertex2d(nextX, nextY);

    _lastDropped = {nextX,nextY};
}

