#ifndef POINTMOTION_H
#define POINTMOTION_H

struct Point {
    double x, y;
};

struct Motion {
    Point frompt;
    Point topt;
};

#endif
