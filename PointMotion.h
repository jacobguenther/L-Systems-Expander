#ifndef POINTMOTION_H
#define POINTMOTION_H

struct Point {
    double x=0.0;
    double y=0.0;
};

struct Motion {
    Point frompt;
    Point topt;
};

#endif
