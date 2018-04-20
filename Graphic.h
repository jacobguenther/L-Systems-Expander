#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "PointMotion.h"
#include <cmath>

class Graphic
{
   public:
	explicit Graphic(const Motion& _m);
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
	Dropgraphic(const Motion& _m, double da, double dd);
    friend class Rulerunner;
    friend class PopCommand;
	void draw() const override;

   private:
    static bool haveapt;
    static Point storedpt;
    bool draws;
};

class Linegraphic : public Graphic {
   public:
    explicit Linegraphic(const Motion &_m) : Graphic(_m) {}
	void draw() const override;
};

class Invisgraphic : public Graphic {
   public:
	void draw() const override;
};

class Rectgraphic : public Graphic {
};
#endif
