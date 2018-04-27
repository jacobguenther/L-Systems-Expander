// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Turtle.h"


void Turtle::flipBy(double multiplier) {
	_states.top()._flipFactor *= multiplier;
}

void Turtle::flip() {
	flipBy(-1.0);
}

void Turtle::rotate(double turnangle) {
	_states.top()._angle += _states.top()._flipFactor * turnangle;
}

void Turtle::forward(double dist) {
	_states.top()._position.x += getscale() * dist
			* cos(DEG2RAD * _states.top()._angle);
	_states.top()._position.y += getscale() * dist
			* sin(DEG2RAD * _states.top()._angle);
}

void Turtle::scaleby(double s) {
	_states.top()._scaleFactor *= s;
}

double Turtle::getscale() const {
	return _states.top()._scaleFactor;
}

int Turtle::getflip() const {
	return _states.top()._flipFactor;
}

void Turtle::setscale(double s) {
	_states.top()._scaleFactor = s;
}

Point Turtle::getposition() {
	return _states.top()._position;
}

void Turtle::push() {
	_states.push(_states.top());
}

void Turtle::pop() {
	_states.pop();
}
