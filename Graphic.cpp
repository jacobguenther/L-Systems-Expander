#include "Graphic.h"

bool Dropgraphic::haveapt = false;
Point Dropgraphic::storedpt;  //!!! Surely there's a better way then static point CMH 4/8/18

void Graphic::draw() const
{
    glBegin(GL_LINES);
    glVertex2d(m.frompt.x, m.frompt.y);
    glVertex2d(m.topt.x, m.topt.y);
    glEnd();
}
