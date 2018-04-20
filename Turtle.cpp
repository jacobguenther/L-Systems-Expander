// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Turtle.h"

using std::make_unique;
using std::cout;
using std::endl;

void Turtle::flipBy(double multiplier) {
	_states.top().flipfac *= multiplier;
}

void Turtle::flip() {
	flipBy(-1.0);
}

void Turtle::rotate(double turnangle) {
	_states.top().angle += _states.top().flipfac * turnangle;
}

void Turtle::forward(double dist) {
	_states.top().position.x += getscale() * dist
			* cos(DEG2RAD * _states.top().angle);
	_states.top().position.y += getscale() * dist
			* sin(DEG2RAD * _states.top().angle);
}

void Turtle::scaleby(double s) {
	_states.top().scalefac *= s;
}

double Turtle::getscale() const {
	return _states.top().scalefac;
}

int Turtle::getflip() const {
	return _states.top().flipfac;
}

void Turtle::setscale(double s) {
	_states.top().scalefac = s;
}

Point Turtle::getposition() {
	return _states.top().position;
}

void Turtle::push() {
	_states.push(_states.top());
}

void Turtle::pop() {
	_states.pop();
}

std::unique_ptr<Graphic> Turtle::draw(const Rule &rule, double flipFactor, double distance) {
    if (rule.drawmethod==Rule::NONE)
        return make_unique<Invisgraphic>();
    Motion temp;
    temp.frompt = getposition();
    forward(distance);
    temp.topt = getposition();
    switch (rule.drawmethod) {
        case Rule::NORM:
            return make_unique<Linegraphic>(temp);
        case Rule::MIDPT:
            return make_unique<Dropgraphic>(temp, 0, .5);
        case Rule::INVIS:
            return make_unique<Invisgraphic>();
        case Rule::RECT:
            return make_unique<Linegraphic>(temp);
        case Rule::DROP:
            return make_unique<Dropgraphic>(temp,
                                            flipFactor* getflip() * rule._dropAngle,
                                            rule._dropDistance);
        case Rule::WRITE:
            cout << "[" << getflip() << "," << getscale() << "] " << rule._info << " distance: " << distance << " flipFactor: " << flipFactor << endl;
            return make_unique<Linegraphic>(temp);
        case Rule::NONE:
            throw(std::logic_error("Turtle::draw - Can't happen, returned already."));
    }
}
