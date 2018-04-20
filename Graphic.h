#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "PointMotion.h"  // just needs struct Motion!!!
#include <cmath>

class Graphic  //how do I make SURE this is an abstract base class?!!!
{
   public:
    explicit Graphic(const Motion &_m) : m(_m) {}
    Graphic()=default;
    virtual void draw() const=0;
    virtual ~Graphic() = default;
    Graphic(const Graphic&) = delete;
    Graphic& operator=(const Graphic&) = delete;
    Graphic(Graphic&&) = delete;
    Graphic& operator=(Graphic&&) = delete;
   protected:
    Motion m;  //not base class because Invisgraphic doesn't need it?!!!
};

class Dropgraphic : public Graphic {
   public:
    Dropgraphic(const Motion &_m, double da, double dd)
        : Graphic(_m), draws(haveapt) {
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
    friend class Rulerunner;
    friend class PopCommand;
    void draw() const override {
        if (draws)
            Graphic::draw();
    }

   private:
    static bool haveapt;
    static Point storedpt;
    bool draws;
};

class Linegraphic : public Graphic {
   public:
    explicit Linegraphic(const Motion &_m) : Graphic(_m) {}
    void draw() const override   {
        Graphic::draw();
    }

};

class Invisgraphic : public Graphic {
   public:
    void draw() const override
    {}
};

class Rectgraphic : public Graphic {
};
#endif
