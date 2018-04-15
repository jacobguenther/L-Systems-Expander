#ifndef TURTLE_H
#define TURTLE_H

//#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI 3.14159265358979323846264338327950288
const double DEG2RAD = M_PI / 180;

#include "PointMotion.h"

class Turtle {
   public:
    void flip() {
        flipfac *= -1;
    }
    void rotate(double turnangle) {
        angle += flipfac * turnangle;
        // angle %= 360;
    }
    void forward(double dist = 1.0) {
        position.x += scalefac * dist * cos(DEG2RAD * angle);
        position.y += scalefac * dist * sin(DEG2RAD * angle);
    }
    void scaleby(double s) {
        scalefac *= s;
    }
    double getscale() const {
        return scalefac;
    }
    int getflip() const {
        return flipfac;
    }
    void setscale(double s) {
        scalefac = s;
    }
    Point getposition() {
        return position;
    }

   private:
    int flipfac{1};
    Point position{0, 0};
    double angle{0};
    double scalefac{1};
};

#endif
