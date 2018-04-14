#ifndef CMD_H
#define CMD_H

#include <string>
#include <list>

#include "Context.h"
#include "Parser.h"
#include "Rulerunner.h"

class Cmd {
   public:
    Cmd() {}
    virtual void execute(Rulerunner *master) = 0;
    virtual ~Cmd() {}
    virtual void cachevalue(const Context &) {}

   private:
    static bool lastconstruct;
};

typedef std::list<std::shared_ptr<Cmd> > Cmdcont;

class Rotatecmd : public Cmd {
   public:
    explicit Rotatecmd(std::shared_ptr<Parsenode> _a) : angle(_a) {}
    virtual void execute(Rulerunner *master) { master->turtles.top().rotate(cachedangle); }
    virtual void cachevalue(const Context &cc) { cachedangle = angle->eval(cc); }

   private:
    std::shared_ptr<Parsenode> angle;
    double cachedangle;
};

class Flipcmd : public Cmd {
   public:
    virtual void execute(Rulerunner *master) { master->turtles.top().flip(); }
};

class Pushcmd : public Cmd {
   public:
    virtual void execute(Rulerunner *master) { master->pushturtle(); }
};

class Popcmd : public Cmd {
   public:
    virtual void execute(Rulerunner *master) {
        master->popturtle();
        Dropgraphic::haveapt = false;
    }
};

class Rulecmd : public Cmd {
   public:
    explicit Rulecmd(const string &_m, bool _r = false, bool _f = false,
            std::shared_ptr<Parsenode> _s = Parser("1").parse())
        : myrule(_m), rev(_r), flip(_f), scale(_s) {}
    virtual void execute(Rulerunner *master) { master->handlerule(myrule, rev, flip, cachedscale); }
    virtual void cachevalue(const Context &cc) { cachedscale = scale->eval(cc); }

   private:
    const string myrule;
    bool rev;
    bool flip;
    std::shared_ptr<Parsenode> scale;
    double cachedscale;
};
#endif
