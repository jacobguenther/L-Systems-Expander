#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "PointMotion.h"  // just needs struct Motion!!!

class Graphic  //how do I make SURE this is an abstract base class?!!!
{
   public:
    Graphic(const Motion &_m) : m(_m) {}
    Graphic()=default;
    virtual void draw() {
        glBegin(GL_LINES);
        glVertex2d(m.frompt.x, m.frompt.y);
        glVertex2d(m.topt.x, m.topt.y);
        glEnd();
    }
    virtual ~Graphic() =default;

   protected:
    Motion m;  //not base class because Invisgraphic doesn't need it?!!!
};

class Dropgraphic : public Graphic {
   public:
    Dropgraphic(const Motion &_m, double da, double dd)
        : Graphic(_m), draws(haveapt) {
        Motion todraw;
        if (haveapt)
            todraw.frompt = storedpt;
        double dx = m.topt.x - m.frompt.x;
        double dy = m.topt.y - m.frompt.y;
        storedpt.x = m.frompt.x + dd * (cos(da) * dx - sin(da) * dy);
        storedpt.y = m.frompt.y + dd * (sin(da) * dx + cos(da) * dy);
        //      storedpt.x = (m.frompt.x+m.topt.x)/2;
        //      storedpt.y = (m.frompt.y+m.topt.y)/2;
        todraw.topt = storedpt;
        haveapt = true;
        m = todraw;
    }
    friend class Rulerunner;
    friend class Popcmd;
    virtual void draw() {
        if (draws) Graphic::draw();
    }

   private:
    static bool haveapt;
    static Point storedpt;
    bool draws;
};

class Linegraphic : public Graphic {
   public:
    Linegraphic(const Motion &_m) : Graphic(_m) {}
};

class Invisgraphic : public Graphic {
   public:
    virtual void draw() {}
};

/*//!!! efficiency?
class Midptgraphic : public Dropgraphic
{
public:
};
*/

class Rectgraphic : public Graphic {
};
#endif
