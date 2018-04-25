//
//  Color.cpp
//  lsystems
//
//  Created by Chris Hartman on 4/24/18.
//

#include "Color.hpp"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

Color Color::colorCircle(double t) {
    Color c;
    c._rgb[0] = float(0.5 + 0.25*( 1.0*cos(t)-sqrt(3)*sin(t)));
    c._rgb[1] = float(0.5 + 0.25*( 1.0*cos(t)+sqrt(3)*sin(t)));
    c._rgb[2] = float(0.5 + 0.25*(-2.0*cos(t)-0.0    *sin(t)));
    return c;
}

void Color::glSet() {
    glColor3fv(_rgb.data());
}
