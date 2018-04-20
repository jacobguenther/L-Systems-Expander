#ifndef TURTLE_H
#define TURTLE_H

#include "Graphic.h"
#include "PointMotion.h"
#include "Rule.h"
#include <cmath>
#include <stack>
#define M_PI 3.14159265358979323846264338327950288
const double DEG2RAD = M_PI / 180;

class Turtle {
   public:
    void flipBy(double multiplier) {
        _states.top().flipfac *= multiplier;
    }
    void flip() {
        flipBy(-1.0);
    }
    void rotate(double turnangle) {
        _states.top().angle += _states.top().flipfac * turnangle;
    }
    void forward(double dist = 1.0) {
        _states.top().position.x += getscale() * dist * cos(DEG2RAD * _states.top().angle);
        _states.top().position.y += getscale() * dist * sin(DEG2RAD * _states.top().angle);
    }
    void scaleby(double s) {
        _states.top().scalefac *= s;
    }
    double getscale() const {
        return _states.top().scalefac;
    }
    int getflip() const {
        return _states.top().flipfac;
    }
    void setscale(double s) {
        _states.top().scalefac = s;
    }
    Point getposition() {
        return _states.top().position;
    }
    void push() {
        _states.push(_states.top());
    }
    void pop() {
        _states.pop();
    }
    std::unique_ptr<Graphic> draw(const Rule &rule, double flipFactor, double distance);
   private:

    struct TurtleState {
        int flipfac{1};
        Point position{0, 0};
        double angle{0};
        double scalefac{1};
    };
    
    std::stack<TurtleState> _states{{ TurtleState{} }};
};

#endif
