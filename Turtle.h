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
	void flipBy(double multiplier);
	void flip();
	void rotate(double turnangle);
	void forward(double dist = 1.0);
	void scaleby(double s);
	double getscale() const;
	int getflip() const;
	void setscale(double s);
	Point getposition();
	void push();
	void pop();
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
