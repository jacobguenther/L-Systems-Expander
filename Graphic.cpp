// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Graphic.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

bool Dropgraphic::haveapt = false;
Point Dropgraphic::storedpt;  //!!! Surely there's a better way then static point CMH 4/8/18

Graphic //how do I make SURE this is an abstract base class?!!!
::Graphic(const Motion& _m) :
		m(_m) {
}

void Graphic::draw() const
{
    glBegin(GL_LINES);
    glVertex2d(m.frompt.x, m.frompt.y);
    glVertex2d(m.topt.x, m.topt.y);
    glEnd();
}

void Dropgraphic::draw() const {
	if (draws)
		Graphic::draw();
}

Dropgraphic::Dropgraphic(const Motion& _m, double da, double dd) :
		Graphic(_m), draws(haveapt) {
	Motion todraw;
	todraw.frompt = storedpt;
	double dx = m.topt.x - m.frompt.x;
	double dy = m.topt.y - m.frompt.y;
	storedpt.x = m.frompt.x + dd * (cos(da) * dx - sin(da) * dy);
	storedpt.y = m.frompt.y + dd * (sin(da) * dx + cos(da) * dy);
	todraw.topt = storedpt;
	haveapt = true;
	m = todraw;
}

void Invisgraphic::draw() const {
}

void Linegraphic::draw() const {
	Graphic::draw();
}
