#ifndef TURTLE_H
#define TURTLE_H

#include <stack>
//#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
const double DEG2RAD = M_PI / 180;

#include "PointMotion.h"

class Turtle {
   public:
    void flip() {
        _states.top().flipfac *= -1;
    }
    void rotate(double turnangle) {
        _states.top().angle += _states.top().flipfac * turnangle;
        // angle %= 360;
    }
    void forward(double dist = 1.0) {
        _states.top().position.x += getscale() * dist * cos(DEG2RAD * _states.top().angle);
        _states.top().position.y += getscale() * dist * sin(DEG2RAD * _states.top().angle);
    }
    void scaleby(double s) {
        _states.top().scalefac *= s;
    }
    double getscale() const {
        return _states.top().scalefac; //!!!unused?
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
   private:

    struct TurtleState {
        int flipfac{1};
        Point position{0, 0};
        double angle{0};
        double scalefac{1};
    };
    
    std::stack<TurtleState> _states{{TurtleState{}}};
};

#endif
