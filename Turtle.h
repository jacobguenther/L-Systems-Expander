#ifndef TURTLE_H
#define TURTLE_H

#include "PointMotion.h"
#include "Rule.h"
#include <cmath>
#include <stack>
#define M_PI 3.14159265358979323846264338327950288
const double DEG2RAD = M_PI / 180;

class Turtle {
   public:
	void flipBy(double multiplier);//!!! double or int or bool?
	void flip();
	void rotate(double turnangle);
	void forward(double dist = 1.0);
	void scaleby(double s);
	double getscale() const;
	int getflip() const;
	void setscale(double s);
    Point getPosition() const;
    double getAngle() const;
	void push();
	void pop();
   private:

    struct TurtleState {
        int _flipFactor{1};
        Point _position{0, 0};
        double _angle{0};
        double _scaleFactor{1};
    };
    
    std::stack<TurtleState> _states{{ TurtleState{} }};
};
#endif
