// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Turtle.h"


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
